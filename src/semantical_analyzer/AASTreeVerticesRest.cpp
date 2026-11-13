#include <vector>

#include "AASTreeVertices.h"


void aast::Entity::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
}
aast::EnclosingEntity* aast::Entity::getEnclosing() {
	return this->enclosing;
}


aast::Entity* aast::Program::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	for (int i = 0; i < this->classDeclarations.size(); i++)
		if (identifier == this->classDeclarations.at(i)->getClassName())
			return this->classDeclarations.at(i);
	return nullptr;
}
aast::Entity* aast::Program::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	Entity* local = this->searchHere(identifier, parameters);
	if (local) return local;
	return nullptr;
}


static bool sameParameters(vector<aast::ParameterDeclaration*>* parameters1, vector<aast::ParameterDeclaration*>* parameters2) {
	if (parameters1->size() != parameters2->size()) return false;
	bool all = true;
	for (int i = 0; i < parameters1->size(); i++) {
		all = all && parameters1->at(i)->getName() == parameters2->at(i)->getName() && parameters1->at(i)->getType() == parameters2->at(i)->getType();
		if (!all) break;
	}
	return all;
}
aast::Entity* aast::ClassBody::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	for (int i = 0; i < this->classStatements.size(); i++) {
		aast::ClassStatement* currentStatement = this->classStatements.at(i);
		if (dynamic_cast<aast::Declaration*>(currentStatement)) {
			aast::Declaration* declaration = dynamic_cast<aast::Declaration*>(currentStatement);
			if (identifier == declaration->getName()) return declaration;
		}
		else if (dynamic_cast<aast::ConstructorDeclaration*>(currentStatement)) {
			aast::ConstructorDeclaration* constructorDeclaration = dynamic_cast<aast::ConstructorDeclaration*>(currentStatement);
			if (sameParameters(constructorDeclaration->getParameterDeclarations(), parameters)) return constructorDeclaration;
		}
		else if (dynamic_cast<aast::MethodDeclaration*>(currentStatement)) {
			aast::MethodDeclaration* methodDeclaration = dynamic_cast<aast::MethodDeclaration*>(currentStatement);
			if (identifier == methodDeclaration->getName()
				&& sameParameters(methodDeclaration->getParameterDeclarations(), parameters)) return methodDeclaration;
		}
	}
	return nullptr;
}
aast::Entity* aast::ClassBody::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	Entity* local = this->searchHere(identifier, parameters);
	if (local) return local;
	return this->enclosing->searchEverywhere(identifier, parameters);
}


aast::Entity* aast::AnyBody::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	for (int i = 0; i < this->statements.size(); i++) {
		aast::Statement* currentStatement = this->statements.at(i);
		if (dynamic_cast<aast::LocalDeclaration*>(currentStatement)) {
			aast::LocalDeclaration* declaration = dynamic_cast<aast::LocalDeclaration*>(currentStatement);
			if (identifier == declaration->getName()) return declaration;
		}
	}
	return nullptr;
}
aast::Entity* aast::AnyBody::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	Entity* local = this->searchHere(identifier, parameters);
	if (local) return local;
	return this->enclosing->searchEverywhere(identifier, parameters);
}


aast::Entity* aast::MethodDeclaration::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	for (int i = 0; i < this->parameterDeclarations.size(); i++)
		if (this->parameterDeclarations.at(i)->getName() == identifier) 
			return this->parameterDeclarations.at(i);
	return nullptr;
}
aast::Entity* aast::MethodDeclaration::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	Entity* local = this->searchHere(identifier, parameters);
	if (local) return local;
	return this->enclosing->searchEverywhere(identifier, parameters);
}


aast::Entity* aast::ConstructorDeclaration::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	for (int i = 0; i < this->parameterDeclarations.size(); i++)
		if (this->parameterDeclarations.at(i)->getName() == identifier) 
			return this->parameterDeclarations.at(i);
	return nullptr;
}
aast::Entity* aast::ConstructorDeclaration::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	Entity* local = this->searchHere(identifier, parameters);
	if (local) return local;
	return this->enclosing->searchEverywhere(identifier, parameters);
}


aast::Entity* aast::Declaration::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	if (identifier == this->name) return this;
	return nullptr;
}
aast::Entity* aast::Declaration::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	Entity* local = this->searchHere(identifier, parameters);
	if (local) return local;
	return this->enclosing->searchEverywhere(identifier, parameters);
}
