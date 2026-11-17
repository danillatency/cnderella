#include "AASTreeVertices.h"


void aast::Entity::optimize() {

}


void aast::Program::optimize() {
	for (int i = 0; i < this->classDeclarations.size(); i++)
		this->classDeclarations.at(i)->optimize();
}


void aast::ClassDeclaration::optimize() {
	this->classBody->optimize();
}


void aast::ClassBody::optimize() {
	for (int i = 0; i < this->classStatements.size(); i++)
		this->classStatements.at(i)->optimize();
}


void aast::MethodDeclaration::optimize() {
	this->body->optimize();
}


void aast::AnyBody::optimize() {
	vector<int> removeThese;
	vector<aast::AnyBody*> bodies;
	for (int i = 0; i < this->statements.size(); i++) {
		aast::IfStatement* ifStatement = dynamic_cast<aast::IfStatement*>(this->statements.at(i));
		if (!ifStatement) continue;
		aast::LiteralBeginningExpression* literalExpression = dynamic_cast<aast::LiteralBeginningExpression*>(ifStatement->getCondition());
		if (!literalExpression) continue;
		if (literalExpression->getAccessesAndCalls()->size()) return;
		if (dynamic_cast<aast::IdentifierBeginningExpression*>(dynamic_cast<aast::Assignment*>(literalExpression->getLiteralCreationStatements()->at(1))->getExpression2())->getIdentifier() == "systemAllocateBoolean_1")
			bodies.push_back(ifStatement->getTrueBody());
		else
			bodies.push_back(ifStatement->getFalseBody());
		removeThese.push_back(i);
	}
	vector<aast::Statement*>* newStatements = new vector<aast::Statement*>();
	for (int i = 0; i < this->statements.size(); i++) {
		int instead = false;
		for (int j = 0; j < removeThese.size(); j++)
			if (removeThese[j] == i) {
				for (int k = 0; k < bodies[j]->getStatements()->size(); k++)
					newStatements->push_back(bodies[j]->getStatements()->at(k));
				instead = true;
			}
		if (instead) continue;
		newStatements->push_back(this->statements.at(i));
	}
	this->statements = *newStatements;
}


void aast::WhileStatement::optimize() {
	this->body->optimize();
}


void aast::ConstructorDeclaration::optimize() {
	this->body->optimize();
}
