#include <vector>
#include <stdexcept>
#include <iostream>

#include "AASTreeVertices.h"
#include "../errors/TerminalErrors.h"


static aast::Access* extractLastAccess(vector<aast::AccessOrCall*>* here) {
	for (int i = here->size() - 1; i > -1; i--) {
		if (dynamic_cast<aast::Access*>(here->at(i)))
			return dynamic_cast<aast::Access*>(here->at(i));
	}
	return nullptr;
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


static vector<logic_error>* updateBtwType(aast::ClassDeclaration*& btwType, string identifier, aast::EnclosingEntity* enclosing) {
	vector<logic_error>* errors = new vector<logic_error>();
	aast::EnclosingEntity* okEnclosing = btwType ? btwType->getClassBody() : enclosing;
	aast::Entity* beginIdentifierDeclaration = btwType ? btwType->getClassBody()->searchHere(identifier, nullptr) : enclosing->searchEverywhere(identifier, nullptr);
	aast::LocalDeclaration* mbLocalDeclaration = dynamic_cast<aast::LocalDeclaration*>(beginIdentifierDeclaration);
	if (mbLocalDeclaration) {
		mbLocalDeclaration->getExpression()->checkTypes();
		btwType = dynamic_cast<aast::ClassDeclaration*>(okEnclosing->searchEverywhere(mbLocalDeclaration->getExpression()->getType(), nullptr));
		if (!btwType) {
			aast::Access* lastAccess = extractLastAccess(mbLocalDeclaration->getExpression()->getAccessesAndCalls());
			errors->push_back(analyzer::Undeclared("entity", lastAccess ? lastAccess->getMultiSpan().begin : mbLocalDeclaration->getExpression()->getMultiSpan().begin));
			return errors;
		}
		return errors;
	}
	aast::Declaration* mbDeclaration = dynamic_cast<aast::Declaration*>(beginIdentifierDeclaration);
	if (mbDeclaration) {
		mbDeclaration->getExpression()->checkTypes();
		btwType = dynamic_cast<aast::ClassDeclaration*>(okEnclosing->searchEverywhere(mbDeclaration->getExpression()->getType(), nullptr));
		if (!btwType) {
			errors->push_back(analyzer::Undeclared("class", mbDeclaration->getMultiSpan().begin));
			return errors;
		}
		return errors;
	}
	aast::ParameterDeclaration* mbParameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(beginIdentifierDeclaration);
	if (mbParameterDeclaration) {
		btwType = dynamic_cast<aast::ClassDeclaration*>(okEnclosing->searchEverywhere(mbParameterDeclaration->getType(), nullptr));
		if (!btwType) {
			errors->push_back(analyzer::Undeclared("parameter class", mbParameterDeclaration->getMultiSpan().begin));
			return errors;
		}
		return errors;
	}
	aast::ClassDeclaration* classDeclaration = dynamic_cast<aast::ClassDeclaration*>(beginIdentifierDeclaration);
	if (!classDeclaration) {
		errors->push_back(analyzer::Undeclared(identifier, Span(0, 0, 0)));
		return errors;
	}
	btwType = classDeclaration;
	return errors;
}


static vector<logic_error>* chainUpdateBtwType(aast::ClassDeclaration*& btwType, vector<aast::AccessOrCall*>& accessesAndCalls, aast::EnclosingEntity* enclosing) {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < accessesAndCalls.size(); i++) {
		aast::Access* access = dynamic_cast<aast::Access*>(accessesAndCalls.at(i));
		aast::Call* call = dynamic_cast<aast::Call*>(accessesAndCalls.at(i));
		aast::Call* furtherCall = dynamic_cast<aast::Call*>(accessesAndCalls.at(i < accessesAndCalls.size() - 1 ? i + 1 : i));
		if (call) combineErrors(errors, call->checkTypes());
		if (furtherCall) combineErrors(errors, furtherCall->checkTypes());
		if (access && furtherCall) {
			if (!btwType) {
				errors->push_back(analyzer::Undeclared("class with " + access->getIdentifier(), access->getMultiSpan().begin));
				return errors;
			}
			i++;
			aast::MethodDeclaration* btwTypeMethod = dynamic_cast<aast::MethodDeclaration*>(btwType->getClassBody()->searchHere(access->getIdentifier(), expressionsToParameterDeclarations(furtherCall->getExpressions())));
			if (!btwTypeMethod) {
				errors->push_back(analyzer::Undeclared("method", access->getMultiSpan().begin));
				return errors;
			}
			aast::ReturnStatement* methodReturn = btwTypeMethod->getAnyBody()->getReturnStatement();
			if (!methodReturn) {
				btwType = nullptr;
				continue;
			}
			// forward declaration
			methodReturn->checkTypes();
			btwType = dynamic_cast<aast::ClassDeclaration*>(enclosing->searchEverywhere(methodReturn->getExpression()->getType(), nullptr));
			continue;
		}
		if (call) continue;
		combineErrors(errors, updateBtwType(btwType, access->getIdentifier(), nullptr));
	}
}


static bool isInheritedFrom(string type1, string type2, aast::EnclosingEntity* enclosing) {
	if (type1 == type2) return true;
	aast::ClassDeclaration* classDeclaration = dynamic_cast<aast::ClassDeclaration*>(enclosing->searchEverywhere(type1, nullptr));
	do {
		classDeclaration = dynamic_cast<aast::ClassDeclaration*>(classDeclaration->getEnclosing()->searchEverywhere(classDeclaration->getClassBaseName(), nullptr));
	}
	while (classDeclaration && classDeclaration->getClassName() != type2);
	if (classDeclaration) return true;
	return false;
}


vector<logic_error>* aast::Program::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classDeclarations.size(); i++)
		combineErrors(errors, this->classDeclarations.at(i)->checkDeclarationSignatures());
	return errors;
}
vector<logic_error>* aast::Program::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classDeclarations.size(); i++)
		combineErrors(errors, this->classDeclarations.at(i)->checkTypes());
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
		combineErrors(errors, this->classStatements.at(i)->checkDeclarationSignatures());
	}
	return errors;
}
vector<logic_error>* aast::ClassBody::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classStatements.size(); i++)
		combineErrors(errors, this->classStatements.at(i)->checkTypes());
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
	combineErrors(errors, this->getAnyBody()->checkDeclarationSignatures());
	return errors;
}
vector<logic_error>* aast::MethodDeclaration::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->parameterDeclarations.size(); i++)
		combineErrors(errors, this->parameterDeclarations.at(i)->checkTypes());
	combineErrors(errors, this->body->checkTypes());
	if (this->body->getReturnStatement() && !this->body->getReturnStatement()->getExpression()->getType().empty()) {
		if (this->type.empty())
			this->type = this->body->getReturnStatement()->getExpression()->getType();
		if (!isInheritedFrom(this->type, this->body->getReturnStatement()->getExpression()->getType(), this->enclosing))
			errors->push_back(analyzer::TypesMismatch(this->type, this->body->getReturnStatement()->getExpression()->getType(), this->body->getReturnStatement()->getExpression()->getAccessesAndCalls()->at(0)->getMultiSpan().begin));
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
			combineErrors(errors, currentIfStatement->getTrueBody()->checkDeclarationSignatures());
			continue;
		}
		aast::WhileStatement* currentWhileStatement = dynamic_cast<aast::WhileStatement*>(this->statements.at(i));
		if (currentWhileStatement) {
			combineErrors(errors, currentWhileStatement->getBody()->checkDeclarationSignatures());
		}
	}
	return errors;
}
vector<logic_error>* aast::AnyBody::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->statements.size(); i++)
		combineErrors(errors, this->statements.at(i)->checkTypes());
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
	combineErrors(errors, this->getAnyBody()->checkDeclarationSignatures());
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
	combineErrors(errors, chainUpdateBtwType(btwType, this->accessesAndCalls, this->enclosing));
	if (btwType)
		this->type = btwType->getClassName();
	return errors;
}


vector<logic_error>* aast::ThisBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	if (!this->accessesAndCalls.size()) {
		this->type = this->enclosing->getThis()->getClassName();
		return errors;
	}
	aast::ClassDeclaration* btwType = this->enclosing->getThis();
	combineErrors(errors, chainUpdateBtwType(btwType, this->accessesAndCalls, this->enclosing));
	if (btwType)
		this->type = btwType->getClassName();
	return errors;
}


vector<logic_error>* aast::BaseBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	if (!this->accessesAndCalls.size()) {
		this->type = this->enclosing->getThis()->getClassBaseName();
		return errors;
	}
	aast::ClassDeclaration* btwType = dynamic_cast<aast::ClassDeclaration*>(this->enclosing->searchEverywhere(this->enclosing->getThis()->getClassBaseName(), nullptr));
	combineErrors(errors, chainUpdateBtwType(btwType, this->accessesAndCalls, this->enclosing));
	if (btwType)
		this->type = btwType->getClassName();
	return errors;
}


vector<logic_error>* aast::LiteralBeginningExpression::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	this->type = dynamic_cast<aast::IdentifierBeginningExpression*>(dynamic_cast<aast::LocalDeclaration*>(this->literalCreationStatements.at(0))->getExpression())->getIdentifier();
	if (!this->accessesAndCalls.size()) return errors;
	aast::ClassDeclaration* btwType = dynamic_cast<aast::ClassDeclaration*>(this->enclosing->searchEverywhere(type, nullptr));
	combineErrors(errors, chainUpdateBtwType(btwType, this->accessesAndCalls, this->enclosing));
	if (btwType)
		this->type = btwType->getClassName();
	return errors;
}


vector<logic_error>* aast::Access::checkTypes() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::Call::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->expressions.size(); i++)
		combineErrors(errors, this->expressions.at(i)->checkTypes());
	return errors;
}


vector<logic_error>* aast::Assignment::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	combineErrors(errors, this->expression1->checkTypes());
	combineErrors(errors, this->expression2->checkTypes());
	if (!this->expression1->getType().empty() && !this->expression2->getType().empty() && !isInheritedFrom(this->expression2->getType(), this->expression1->getType(), this->enclosing))
		errors->push_back(analyzer::TypesMismatch(this->expression1->getType(), this->expression2->getType(), this->expression1->getMultiSpan().begin));
	return errors;
}


vector<logic_error>* aast::IfStatement::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	combineErrors(errors, this->condition->checkTypes());
	if (this->condition->getType() != "Boolean") {
		vector<aast::AccessOrCall*>* chain = this->condition->getAccessesAndCalls();
		int lastAccessIndex = this->condition->getAccessesAndCalls()->size() - 1;
		for (; lastAccessIndex > 0; lastAccessIndex--)
			if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
				break;
		errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
		return errors;
	}
	combineErrors(errors, this->trueBody->checkTypes());
	combineErrors(errors, this->falseBody->checkTypes());
	return errors;
}


vector<logic_error>* aast::WhileStatement::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	combineErrors(errors, this->condition->checkTypes());
	if (this->condition->getType() != "Boolean") {
		vector<aast::AccessOrCall*>* chain = this->condition->getAccessesAndCalls();
		int lastAccessIndex = this->condition->getAccessesAndCalls()->size() - 1;
		for (; lastAccessIndex > 0; lastAccessIndex--)
			if (dynamic_cast<aast::Access*>(chain->at(lastAccessIndex)))
				break;
		errors->push_back(analyzer::Undeclared("element", chain->at(lastAccessIndex)->getMultiSpan().begin));
		return errors;
	}
	combineErrors(errors, this->body->checkTypes());
	return errors;
}


vector<logic_error>* aast::ReturnStatement::checkTypes() {
	return this->expression->checkTypes();
}


vector<logic_error>* aast::ConstructorDeclaration::checkTypes() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->parameterDeclarations.size(); i++)
		combineErrors(errors, this->parameterDeclarations.at(i)->checkTypes());
	combineErrors(errors, this->body->checkTypes());
	return errors;
}
