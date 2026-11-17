#include <vector>

#include "AASTreeVertices.h"
#include "../errors/TerminalErrors.h"


void aast::Entity::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
}

aast::EnclosingEntity* aast::Entity::getEnclosing() {
	return this->enclosing;
}


void aast::ClassBody::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	for (int i = 0; i < this->classStatements.size(); i++)
		this->classStatements.at(i)->setEnclosing(this);
}


void aast::MethodDeclaration::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	for (int i = 0; i < this->parameterDeclarations.size(); i++)
		this->parameterDeclarations.at(i)->setEnclosing(this);
	this->body->setEnclosing(this);
}


void aast::ConstructorDeclaration::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->body->setEnclosing(this);
	for (int i = 0; i < this->parameterDeclarations.size(); i++)
		this->parameterDeclarations.at(i)->setEnclosing(this);
}


void aast::Declaration::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->expression->setEnclosing(entity);
}


void aast::AnyBody::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	for (int i = 0; i < this->statements.size(); i++)
		this->statements.at(i)->setEnclosing(this);
}


void aast::ReturnStatement::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->expression->setEnclosing(entity);
}


void aast::Expression::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	for (int i = 0; i < this->accessesAndCalls.size(); i++)
		this->accessesAndCalls.at(i)->setEnclosing(entity);
}


void aast::Call::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	for (int i = 0; i < this->expressions.size(); i++)
		this->expressions.at(i)->setEnclosing(entity);
}


void aast::LocalDeclaration::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->expression->setEnclosing(entity);
}


void aast::Assignment::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->expression1->setEnclosing(entity);
	this->expression2->setEnclosing(entity);
}


void aast::IfStatement::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->condition->setEnclosing(entity);
	this->trueBody->setEnclosing(entity);
	this->falseBody->setEnclosing(entity);
}

void aast::WhileStatement::setEnclosing(aast::EnclosingEntity* entity) {
	this->enclosing = entity;
	this->condition->setEnclosing(entity);
	this->body->setEnclosing(entity);
}


aast::ClassDeclaration* aast::EnclosingEntity::getThis() {
	aast::ClassBody* classBody = dynamic_cast<aast::ClassBody*>(this);
	if (classBody) return dynamic_cast<aast::ClassDeclaration*>(classBody->getEnclosing());
	EnclosingEntity* entity = this;
	do {
		entity = entity->enclosing;
		classBody = dynamic_cast<aast::ClassBody*>(entity);
	} while (!classBody);
	return dynamic_cast<aast::ClassDeclaration*>(classBody->getEnclosing());
}


aast::Entity* aast::Program::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	for (int i = 0; i < this->classDeclarations.size(); i++)
		if (identifier == this->classDeclarations.at(i)->getClassName())
			return this->classDeclarations.at(i);
	return nullptr;
}
aast::Entity* aast::Program::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	return this->searchHere(identifier, parameters);
}


aast::Entity* aast::ClassDeclaration::searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	return this->classBody->searchHere(identifier, parameters);
}
aast::Entity* aast::ClassDeclaration::searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) {
	return this->enclosing->searchEverywhere(identifier, parameters);
}


static bool sameParameters(vector<aast::ParameterDeclaration*>* parameters1, vector<aast::ParameterDeclaration*>* parameters2) {
	if (parameters1->size() != parameters2->size()) return false;
	bool all = true;
	for (int i = 0; i < parameters1->size(); i++) {
		all = all && parameters1->at(i)->getType() == parameters2->at(i)->getType();
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
		else if (parameters != nullptr) {
			if (identifier.empty() && dynamic_cast<aast::ConstructorDeclaration*>(currentStatement)) {
				aast::ConstructorDeclaration* constructorDeclaration = dynamic_cast<aast::ConstructorDeclaration*>(currentStatement);
				if (sameParameters(constructorDeclaration->getParameterDeclarations(), parameters)) return constructorDeclaration;
			}
			else if (dynamic_cast<aast::MethodDeclaration*>(currentStatement)) {
				aast::MethodDeclaration* methodDeclaration = dynamic_cast<aast::MethodDeclaration*>(currentStatement);
				if (identifier == methodDeclaration->getName()
					&& sameParameters(methodDeclaration->getParameterDeclarations(), parameters)) return methodDeclaration;
			}
		}
	}
	if (this->getThis()->getClassBaseName().empty()) return nullptr;
	aast::Entity* baseBody = this->enclosing->searchEverywhere(this->getThis()->getClassBaseName(), nullptr);
	if (!baseBody) throw analyzer::Undeclared("base class", this->getThis()->getMultiSpan().begin);
	return dynamic_cast<aast::ClassDeclaration*>(baseBody)->getClassBody()->searchHere(identifier, parameters);
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
