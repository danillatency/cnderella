#include <vector>
#include <stdexcept>

#include "AASTreeVertices.h"
#include "../errors/TerminalErrors.h"


vector<logic_error>* aast::Program::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	for (int i = 0; i < this->classDeclarations.size(); i++) {
		aast::ClassDeclaration* foundDeclaration = dynamic_cast<aast::ClassDeclaration*>(this->searchHere(this->classDeclarations.at(i)->getClassName(), nullptr));
		if (foundDeclaration != this->classDeclarations.at(i))
			errors->push_back(analyzer::Redeclaration("class", foundDeclaration->getMultiSpan().begin, this->classDeclarations.at(i)->getMultiSpan().begin));
		vector<logic_error>* nestedErrors = this->classDeclarations.at(i)->getClassBody()->checkDeclarationSignatures();
		errors->insert(errors->end(), nestedErrors->begin(), nestedErrors->end());
	}
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