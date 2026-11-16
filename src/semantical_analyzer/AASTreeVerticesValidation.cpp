#include <vector>
#include <stdexcept>

#include "AASTreeVertices.h"
#include "../errors/TerminalErrors.h"


static aast::Access* extractLastAccess(vector<aast::AccessOrCall*>* here) {
	for (int i = here->size() - 1; i > -1; i--) {
		if (dynamic_cast<aast::Access*>(here->at(i)))
			return dynamic_cast<aast::Access*>(here->at(i));
	}
}


static void combineErrors(vector<logic_error>* errors, vector<logic_error>* nestedErrors) {
	errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
}


static vector<aast::ParameterDeclaration*>* expressionsToParameterDeclarations(vector<aast::Expression*>& expressions) {
	vector<aast::ParameterDeclaration*>* parameters = new vector<aast::ParameterDeclaration*>();
	for (int i = 0; i < expressions.size(); i++) {
		aast::ParameterDeclaration* next = new aast::ParameterDeclaration();
		next->setType(expressions.at(i)->getType());
		parameters->push_back(next);
	}
	return parameters;
}


static vector<logic_error>* updateBtwType(aast::ClassDeclaration* btwType, string identifier, aast::EnclosingEntity* enclosing) {
	vector<logic_error>* errors = new vector<logic_error>();
	aast::Entity* beginIdentifierDeclaration = btwType ? btwType->getClassBody()->searchHere(identifier, nullptr) : enclosing->searchEverywhere(identifier, nullptr);
	aast::LocalDeclaration* mbLocalDeclaration = dynamic_cast<aast::LocalDeclaration*>(beginIdentifierDeclaration);
	if (mbLocalDeclaration) {
		btwType = dynamic_cast<aast::ClassDeclaration*>(enclosing->searchEverywhere(mbLocalDeclaration->getExpression()->getType(), nullptr));
		if (!btwType) {
			errors->push_back(analyzer::Undeclared("entity", extractLastAccess(mbLocalDeclaration->getExpression()->getAccessesAndCalls())->getMultiSpan().begin));
			return errors;
		}
		return errors;
	}
	aast::Declaration* mbDeclaration = dynamic_cast<aast::Declaration*>(beginIdentifierDeclaration);
	if (mbDeclaration) {
		btwType = dynamic_cast<aast::ClassDeclaration*>(enclosing->searchEverywhere(mbDeclaration->getExpression()->getType(), nullptr));
		if (!btwType) {
			errors->push_back(analyzer::Undeclared("class", mbDeclaration->getMultiSpan().begin));
			return errors;
		}
		return errors;
	}
	aast::ParameterDeclaration* mbParameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(beginIdentifierDeclaration);
	if (mbParameterDeclaration) {
		btwType = dynamic_cast<aast::ClassDeclaration*>(enclosing->searchEverywhere(mbParameterDeclaration->getType(), nullptr));
		if (!btwType) {
			errors->push_back(analyzer::Undeclared("parameter class", mbParameterDeclaration->getMultiSpan().begin));
			return errors;
		}
		return errors;
	}
	aast::ClassDeclaration* classDeclaration = dynamic_cast<aast::ClassDeclaration*>(beginIdentifierDeclaration);
	if (!classDeclaration) {
		errors->push_back(analyzer::Undeclared("identifier class", beginIdentifierDeclaration->getMultiSpan().begin));
		return errors;
	}
	btwType = classDeclaration;
	return errors;
}


static vector<logic_error>* chainUpdateBtwType(aast::ClassDeclaration* btwType, vector<aast::AccessOrCall*>& accessesAndCalls, aast::EnclosingEntity* enclosing) {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < accessesAndCalls.size(); i++) {
		aast::Access* access = dynamic_cast<aast::Access*>(accessesAndCalls.at(i));
		aast::Call* call = dynamic_cast<aast::Call*>(accessesAndCalls.at(i < accessesAndCalls.size() - 1 ? i + 1 : i));
		if (call) combineErrors(errors, call->checkTypes());
		if (access && call) {
			aast::MethodDeclaration* btwTypeMethod = dynamic_cast<aast::MethodDeclaration*>(btwType->getClassBody()->searchHere(access->getIdentifier(), expressionsToParameterDeclarations(call->getExpressions())));
			if (!btwTypeMethod) {
				errors->push_back(analyzer::Undeclared("method", access->getMultiSpan().begin));
				return errors;
			}
			aast::ReturnStatement* methodReturn = btwTypeMethod->getAnyBody()->getReturnStatement();
			if (!methodReturn) {
				btwType = nullptr;
				continue;
			}
			btwType = dynamic_cast<aast::ClassDeclaration*>(enclosing->searchEverywhere(methodReturn->getExpression()->getType(), nullptr));
			continue;
		}
		if (call) continue;
		combineErrors(errors, updateBtwType(btwType, access->getIdentifier(), nullptr));
	}

}


vector<logic_error>* aast::Program::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classDeclarations.size(); i++)
		combineErrors(errors, this->classDeclarations.at(i)->checkDeclarationSignatures());
	return errors;
}
vector<logic_error>* aast::Program::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classDeclarations.size(); i++) {
		vector<logic_error>* nestedErrors = this->classDeclarations.at(i)->checkTypes();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	return errors;
}


vector<logic_error>* aast::ClassDeclaration::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	aast::ClassDeclaration* foundDeclaration = dynamic_cast<aast::ClassDeclaration*>(this->enclosing->searchHere(this->name, nullptr));
	if (foundDeclaration != this)
		errors->push_back(analyzer::Redeclaration("class", foundDeclaration->getMultiSpan().begin, this->getMultiSpan().begin));
	combineErrors(errors, this->classBody->checkDeclarationSignatures());
	return errors;
}


vector<logic_error>* aast::ClassBody::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classStatements.size(); i++) {
		if (dynamic_cast<aast::MethodDeclaration*>(this->classStatements.at(i))) {
			aast::MethodDeclaration* currentMethod = dynamic_cast<aast::MethodDeclaration*>(this->classStatements.at(i));
			aast::MethodDeclaration* foundMethod = dynamic_cast<aast::MethodDeclaration*>(this->searchHere(currentMethod->getName(), currentMethod->getParameterDeclarations()));
			if (foundMethod != currentMethod)
				errors->push_back(analyzer::Redeclaration("method", foundMethod->getMultiSpan().begin, currentMethod->getMultiSpan().begin));
		}
		else if (dynamic_cast<aast::ConstructorDeclaration*>(this->classStatements.at(i))) {
			aast::ConstructorDeclaration* currentConstructor = dynamic_cast<aast::ConstructorDeclaration*>(this->classStatements.at(i));
			aast::ConstructorDeclaration* foundConstructor = dynamic_cast<aast::ConstructorDeclaration*>(this->searchHere("", currentConstructor->getParameterDeclarations()));
			if (foundConstructor != currentConstructor)
				errors->push_back(analyzer::Redeclaration("constructor", foundConstructor->getMultiSpan().begin, currentConstructor->getMultiSpan().begin));
		}
		else {
			aast::Declaration* currentDeclaration = dynamic_cast<aast::Declaration*>(this->classStatements.at(i));
			aast::Declaration* foundDeclaration = dynamic_cast<aast::Declaration*>(this->searchHere(currentDeclaration->getName(), nullptr));
			if (foundDeclaration != currentDeclaration)
				errors->push_back(analyzer::Redeclaration("declaration", foundDeclaration->getMultiSpan().begin, currentDeclaration->getMultiSpan().begin));
		}
		vector<logic_error>* nestedErrors = this->classStatements.at(i)->checkDeclarationSignatures();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	return errors;
}
vector<logic_error>* aast::ClassBody::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classStatements.size(); i++) {
		vector<logic_error>* nestedErrors = this->classStatements.at(i)->checkTypes();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	return errors;
}


vector<logic_error>* aast::MethodDeclaration::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->parameterDeclarations.size(); i++) {
		aast::ParameterDeclaration* currentParameterDeclaration = this->parameterDeclarations.at(i);
		aast::ParameterDeclaration* foundParameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(this->searchHere(currentParameterDeclaration->getName(), nullptr));
		if (foundParameterDeclaration != currentParameterDeclaration)
			errors->push_back(analyzer::Redeclaration("parameter", foundParameterDeclaration->getMultiSpan().begin, currentParameterDeclaration->getMultiSpan().begin));
	}
	vector<logic_error>* nestedErrors = this->getAnyBody()->checkDeclarationSignatures();
	errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	return errors;
}
vector<logic_error>* aast::MethodDeclaration::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->parameterDeclarations.size(); i++) {
		vector<logic_error>* nestedErrors = this->parameterDeclarations.at(i)->checkTypes();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	vector<logic_error>* nestedErrors = this->body->checkTypes();
	errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	if (this->body->getReturnStatement()) {
		if (this->type.empty())
			this->type = this->body->getReturnStatement()->getExpression()->getType();
		if (this->type != this->body->getReturnStatement()->getExpression()->getType())
			errors->push_back(analyzer::TypesMismatch(this->type, this->body->getReturnStatement()->getExpression()->getType(), this->body->getReturnStatement()->getMultiSpan().begin));
	}
	return errors;
}


vector<logic_error>* aast::AnyBody::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->statements.size(); i++) {
		if (dynamic_cast<aast::LocalDeclaration*>(this->statements.at(i))) {
			aast::LocalDeclaration* currentLocalDeclaration = dynamic_cast<aast::LocalDeclaration*>(this->statements.at(i));
			aast::LocalDeclaration* foundLocalDeclaration = dynamic_cast<aast::LocalDeclaration*>(this->searchHere(currentLocalDeclaration->getName(), nullptr));
			if (foundLocalDeclaration != currentLocalDeclaration)
				errors->push_back(analyzer::Redeclaration("local declaration", foundLocalDeclaration->getMultiSpan().begin, currentLocalDeclaration->getMultiSpan().begin));
			continue;
		}
		aast::IfStatement* currentIfStatement = dynamic_cast<aast::IfStatement*>(this->statements.at(i));
		if (currentIfStatement) {
			vector<logic_error>* nestedErrors = currentIfStatement->getTrueBody()->checkDeclarationSignatures();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			continue;
		}
		aast::WhileStatement* currentWhileStatement = dynamic_cast<aast::WhileStatement*>(this->statements.at(i));
		if (currentWhileStatement) {
			vector<logic_error>* nestedErrors = currentWhileStatement->getBody()->checkDeclarationSignatures();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
		}
	}
	return errors;
}
vector<logic_error>* aast::AnyBody::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->statements.size(); i++) {
		vector<logic_error>* nestedErrors = this->statements.at(i)->checkTypes();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	return errors;
}
aast::ReturnStatement* aast::AnyBody::getReturnStatement() {
	for (int i = 0; i < this->statements.size(); i++) {
		aast::ReturnStatement* statement = dynamic_cast<aast::ReturnStatement*>(this->statements.at(i));
		if (statement) return statement;
	}
	return nullptr;
}


vector<logic_error>* aast::ConstructorDeclaration::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->parameterDeclarations.size(); i++) {
		aast::ParameterDeclaration* currentParameterDeclaration = this->parameterDeclarations.at(i);
		aast::ParameterDeclaration* foundParameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(this->searchHere(currentParameterDeclaration->getName(), nullptr));
		if (foundParameterDeclaration != currentParameterDeclaration)
			errors->push_back(analyzer::Redeclaration("parameter", foundParameterDeclaration->getMultiSpan().begin, currentParameterDeclaration->getMultiSpan().begin));
	}
	vector<logic_error>* nestedErrors = this->getAnyBody()->checkDeclarationSignatures();
	errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	return errors;
}


vector<logic_error>* aast::Declaration::checkDeclarationSignatures() {
	return new vector<logic_error>();
}
vector<logic_error>* aast::Declaration::checkTypes() {
	return this->expression->checkTypes();
}


vector<logic_error>* aast::ClassDeclaration::checkTypes() {
	return this->classBody->checkTypes();
}


vector<logic_error>* aast::ParameterDeclaration::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	aast::ClassDeclaration* type = dynamic_cast<aast::ClassDeclaration*>(this->enclosing->searchEverywhere(this->type, nullptr));
	if (!type)
		errors->push_back(analyzer::Undeclared("type of a parameter", this->multiSpan->end));
	return errors;
}


vector<logic_error>* aast::LocalDeclaration::checkTypes() {
	return this->expression->checkTypes();
}



vector<logic_error>* aast::IdentifierBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	if (this->identifier.rfind("system", 0) == 0) return errors;
	aast::ClassDeclaration* btwType = nullptr;
	combineErrors(errors, updateBtwType(btwType, this->identifier, this->enclosing));
	if (errors->size()) return errors;
	chainUpdateBtwType(btwType, this->accessesAndCalls, this->enclosing);
	this->type = btwType->getClassName();
	return errors;
}


vector<logic_error>* aast::ThisBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	if (!this->accessesAndCalls.size()) {
		this->type = this->enclosing->getThis()->getClassName();
		return errors;
	}
	aast::ClassDeclaration* inferredType = this->enclosing->getThis();
	
	this->type = inferredType->getClassName();
	return errors;
}


vector<logic_error>* aast::BaseBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	if (!this->accessesAndCalls.size()) {
		this->type = this->enclosing->getThis()->getClassBaseName();
		return errors;
	}
	aast::ClassDeclaration* inferredType;
	aast::Access* access = dynamic_cast<aast::Access*>(this->accessesAndCalls.at(0));
	aast::Call* call = dynamic_cast<aast::Call*>(this->accessesAndCalls.at(this->accessesAndCalls.size() > 1 ? 1 : 0));
	if (call) {
		vector<logic_error>* nestedErrors = call->checkTypes();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	if (access && call) {
		aast::Entity* foundBaseClass = this->enclosing->searchEverywhere(this->enclosing->getThis()->getClassBaseName(), nullptr);
		aast::ClassDeclaration* inferredClass = dynamic_cast<aast::ClassDeclaration*>(foundBaseClass);
		if (!inferredClass) {
			errors->push_back(analyzer::Undeclared("class", this->enclosing->getThis()->getMultiSpan().begin));
			return errors;
		}
		aast::Entity* foundMethod = inferredClass->getClassBody()->searchHere("", expressionsToParameterDeclarations(call->getExpressions()));
		aast::MethodDeclaration* inferredMethod = dynamic_cast<aast::MethodDeclaration*>(foundMethod);
		if (!inferredMethod) {
			errors->push_back(analyzer::Undeclared("method", access->getMultiSpan().begin));
			return errors;
		}
		aast::ReturnStatement* methodReturn = inferredMethod->getAnyBody()->getReturnStatement();
		if (!methodReturn) {
			inferredType = nullptr;
		}
		else {
			vector<logic_error>* nestedErrors = methodReturn->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = this->enclosing->searchEverywhere(methodReturn->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = methodReturn->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = methodReturn->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--)
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
		}
	}
	for (int i = 0; i < this->accessesAndCalls.size(); i++) {
		aast::Access* access = dynamic_cast<aast::Access*>(this->accessesAndCalls.at(i));
		aast::Call* call = dynamic_cast<aast::Call*>(this->accessesAndCalls.at(i < this->accessesAndCalls.size() - 1 ? i + 1 : i));
		if (call) {
			vector<logic_error>* nestedErrors = call->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
		}
		else continue;
		if (access && call) {
			aast::Entity* methodFoundInInferredType = inferredType->getClassBody()->searchHere(access->getIdentifier(), expressionsToParameterDeclarations(call->getExpressions()));
			aast::MethodDeclaration* inferredMethod = dynamic_cast<aast::MethodDeclaration*>(methodFoundInInferredType);
			if (!inferredMethod) {
				errors->push_back(analyzer::Undeclared("method", access->getMultiSpan().begin));
				return errors;
			}
			aast::ReturnStatement* methodReturn = inferredMethod->getAnyBody()->getReturnStatement();
			if (!methodReturn) {
				inferredType = nullptr;
				continue;
			}
			vector<logic_error>* nestedErrors = methodReturn->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = this->enclosing->searchEverywhere(methodReturn->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = methodReturn->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = methodReturn->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--)
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
			continue;
		}
		aast::Entity* foundIdentifier = this->enclosing->searchEverywhere(access->getIdentifier(), nullptr);
		aast::LocalDeclaration* localDeclaration = dynamic_cast<aast::LocalDeclaration*>(foundIdentifier);
		aast::Declaration* declaration = dynamic_cast<aast::Declaration*>(foundIdentifier);
		aast::ParameterDeclaration* parameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(foundIdentifier);
		aast::ClassDeclaration* classDeclaration = dynamic_cast<aast::ClassDeclaration*>(foundIdentifier);
		aast::ClassDeclaration* inferredType;
		if (localDeclaration) {
			vector<logic_error>* nestedErrors = localDeclaration->getExpression()->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = localDeclaration->getEnclosing()->searchEverywhere(localDeclaration->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = localDeclaration->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = localDeclaration->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--) {
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				}
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
		}
		else if (declaration) {
			vector<logic_error>* nestedErrors = declaration->getExpression()->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = declaration->getEnclosing()->searchEverywhere(declaration->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = declaration->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = declaration->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--)
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
		}
		else if (parameterDeclaration) {
			aast::Entity* foundType = parameterDeclaration->getEnclosing()->searchEverywhere(parameterDeclaration->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				errors->push_back(analyzer::Undeclared("class", parameterDeclaration->getMultiSpan().end));
				return errors;
			}
		}
		else if (!classDeclaration) {
			errors->push_back(analyzer::Undeclared("element", this->multiSpan->begin));
			return errors;
		}
	}
	this->type = inferredType->getClassName();
	return errors;
}


vector<logic_error>* aast::LiteralBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	string type = dynamic_cast<aast::IdentifierBeginningExpression*>(dynamic_cast<aast::LocalDeclaration*>(this->literalCreationStatements.at(0))->getExpression())->getIdentifier();
	if (!this->accessesAndCalls.size()) {
		this->type = type;
		return errors;
	}
	aast::ClassDeclaration* inferredType = dynamic_cast<aast::ClassDeclaration*>(this->enclosing->searchEverywhere(type, nullptr));
	for (int i = 0; i < this->accessesAndCalls.size(); i++) {
		aast::Access* access = dynamic_cast<aast::Access*>(this->accessesAndCalls.at(i));
		aast::Call* call = dynamic_cast<aast::Call*>(this->accessesAndCalls.at(i < this->accessesAndCalls.size() - 1 ? i + 1 : i));
		if (call) {
			vector<logic_error>* nestedErrors = call->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
		}
		else continue;
		if (access && call) {
			aast::Entity* methodFoundInInferredType = inferredType->getClassBody()->searchHere(access->getIdentifier(), expressionsToParameterDeclarations(call->getExpressions()));
			aast::MethodDeclaration* inferredMethod = dynamic_cast<aast::MethodDeclaration*>(methodFoundInInferredType);
			if (!inferredMethod) {
				errors->push_back(analyzer::Undeclared("method", access->getMultiSpan().begin));
				return errors;
			}
			aast::ReturnStatement* methodReturn = inferredMethod->getAnyBody()->getReturnStatement();
			if (!methodReturn) {
				inferredType = nullptr;
				continue;
			}
			vector<logic_error>* nestedErrors = methodReturn->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = this->enclosing->searchEverywhere(methodReturn->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = methodReturn->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = methodReturn->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--)
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
			continue;
		}
		aast::Entity* foundIdentifier = this->enclosing->searchEverywhere(access->getIdentifier(), nullptr);
		aast::LocalDeclaration* localDeclaration = dynamic_cast<aast::LocalDeclaration*>(foundIdentifier);
		aast::Declaration* declaration = dynamic_cast<aast::Declaration*>(foundIdentifier);
		aast::ParameterDeclaration* parameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(foundIdentifier);
		aast::ClassDeclaration* classDeclaration = dynamic_cast<aast::ClassDeclaration*>(foundIdentifier);
		aast::ClassDeclaration* inferredType;
		if (localDeclaration) {
			vector<logic_error>* nestedErrors = localDeclaration->getExpression()->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = localDeclaration->getEnclosing()->searchEverywhere(localDeclaration->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = localDeclaration->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = localDeclaration->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--) {
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				}
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
		}
		else if (declaration) {
			vector<logic_error>* nestedErrors = declaration->getExpression()->checkTypes();
			errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
			aast::Entity* foundType = declaration->getEnclosing()->searchEverywhere(declaration->getExpression()->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				vector<aast::AccessOrCall*>* chain = declaration->getExpression()->getAccessesAndCalls();
				int lastAccessIndex = declaration->getExpression()->getAccessesAndCalls()->size() - 1;
				for (; lastAccessIndex > 0; lastAccessIndex--)
					if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
						break;
				errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
				return errors;
			}
		}
		else if (parameterDeclaration) {
			aast::Entity* foundType = parameterDeclaration->getEnclosing()->searchEverywhere(parameterDeclaration->getType(), nullptr);
			inferredType = dynamic_cast<aast::ClassDeclaration*>(foundType);
			if (!inferredType) {
				errors->push_back(analyzer::Undeclared("class", parameterDeclaration->getMultiSpan().end));
				return errors;
			}
		}
		else if (!classDeclaration) {
			errors->push_back(analyzer::Undeclared("element", this->multiSpan->begin));
			return errors;
		}
	}
	this->type = inferredType->getClassName();
	return errors;
}


vector<logic_error>* aast::Access::checkTypes() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::Call::checkTypes() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::Assignment::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	vector<logic_error>* nestedErrors1 = this->expression1->checkTypes();
	vector<logic_error>* nestedErrors2 = this->expression2->checkTypes();
	errors->insert(errors->end(), nestedErrors1->begin(), nestedErrors1->end());
	errors->insert(errors->end(), nestedErrors2->begin(), nestedErrors2->end());
	if (this->expression1->getType() != this->expression2->getType())
		errors->push_back(analyzer::TypesMismatch(this->expression1->getType(), this->expression2->getType(), this->getMultiSpan().begin));
	return errors;
}


vector<logic_error>* aast::IfStatement::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	vector<logic_error>* nestedErrorsCondition = this->condition->checkTypes();
	if (this->condition->getType() != "Boolean") {
		vector<aast::AccessOrCall*>* chain = this->condition->getAccessesAndCalls();
		int lastAccessIndex = this->condition->getAccessesAndCalls()->size() - 1;
		for (; lastAccessIndex > 0; lastAccessIndex--)
			if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
				break;
		errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
		return errors;
	}
	vector<logic_error>* nestedErrorsTrue = this->trueBody->checkTypes();
	vector<logic_error>* nestedErrorsFalse = this->falseBody->checkTypes();
	errors->insert(errors->end(), nestedErrorsTrue->begin(), nestedErrorsTrue->begin());
	errors->insert(errors->end(), nestedErrorsFalse->begin(), nestedErrorsFalse->begin());
	return errors;
}


vector<logic_error>* aast::WhileStatement::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	vector<logic_error>* nestedErrorsCondition = this->condition->checkTypes();
	if (this->condition->getType() != "Boolean") {
		vector<aast::AccessOrCall*>* chain = this->condition->getAccessesAndCalls();
		int lastAccessIndex = this->condition->getAccessesAndCalls()->size() - 1;
		for (; lastAccessIndex > 0; lastAccessIndex--)
			if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
				break;
		errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
		return errors;
	}
	vector<logic_error>* nestedErrors = this->body->checkTypes();
	errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->begin());
	return errors;
}


vector<logic_error>* aast::ReturnStatement::checkTypes() {
	return this->expression->checkTypes();
}


vector<logic_error>* aast::ConstructorDeclaration::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->parameterDeclarations.size(); i++) {
		vector<logic_error>* nestedErrors = this->parameterDeclarations.at(i)->checkTypes();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
	vector<logic_error>* nestedErrors = this->body->checkTypes();
	errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	return errors;
}
