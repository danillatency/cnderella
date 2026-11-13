#include "AASTreeVertices.h"


void aast::Program::addClassDeclaration(aast::ClassDeclaration* classDeclaration) {
	this->classDeclarations.push_back(classDeclaration);
}


void aast::ClassDeclaration::setClassName(string name) {
	this->name = name;
}
void aast::ClassDeclaration::setClassBaseName(string baseName) {
	this->baseName = baseName;
}
void aast::ClassDeclaration::setClassBody(aast::ClassBody* classBody) {
	this->classBody = classBody;
}


void aast::ClassBody::addClassStatement(aast::ClassStatement* classStatement) {
	this->classStatements.push_back(classStatement);
}


void aast::MethodDeclaration::setName(string name) {
	this->name = name;
}
void aast::MethodDeclaration::setType(string type) {
	this->type = type;
}
void aast::MethodDeclaration::setAnyBody(aast::AnyBody* body) {
	this->body = body;
}
void aast::MethodDeclaration::addParameterDeclaration(aast::ParameterDeclaration* parameterDeclaration) {
	this->parameterDeclarations.push_back(parameterDeclaration);
}


void aast::ParameterDeclaration::setName(string name) {
	this->name = name;
}
void aast::ParameterDeclaration::setType(string type) {
	this->type = type;
}


void aast::AnyBody::addStatement(aast::Statement* statement) {
	this->statements.push_back(statement);
}


void aast::LocalDeclaration::setName(string name) {
	this->name = name;
}
void aast::LocalDeclaration::setExpression(aast::Expression* expression) {
	this->expression = expression;
}


void aast::Expression::addAccessOrCall(aast::AccessOrCall* accessOrCall) {
	this->accessesAndCalls.push_back(accessOrCall);
}


void aast::IdentifierBeginningExpression::setIdentifier(string identifier) {
	this->identifier = identifier;
}


void aast::LiteralBeginningExpression::addLiteralCreationStatement(aast::Statement* statement) {
	this->literalCreationStatements.push_back(statement);
}


void aast::Access::setIdentifier(string identifier) {
	this->identifier = identifier;
}


void aast::Call::addExpression(aast::Expression* expression) {
	this->expressions.push_back(expression);
}


void aast::Assignment::setExpression1(aast::Expression* expression1) {
	this->expression1 = expression1;
}
void aast::Assignment::setExpression2(aast::Expression* expression2) {
	this->expression2 = expression2;
}


void aast::IfStatement::setCondition(aast::Expression* condition) {
	this->condition = condition;
}
void aast::IfStatement::addTrueStatement(aast::Statement* statement) {
	this->trueStatements.push_back(statement);
}
void aast::IfStatement::addFalseStatement(aast::Statement* statement) {
	this->falseStatements.push_back(statement);
}


void aast::WhileStatement::setCondition(aast::Expression* condition) {
	this->condition = condition;
}
void aast::WhileStatement::addStatement(aast::Statement* statement) {
	this->statements.push_back(statement);
}


void aast::ReturnStatement::setExpression(aast::Expression* expression) {
	this->expression = expression;
}


void aast::ConstructorDeclaration::setAnyBody(aast::AnyBody* body) {
	this->body = body;
}
void aast::ConstructorDeclaration::addParameterDeclaration(aast::ParameterDeclaration* parameterDeclaration) {
	this->parameterDeclarations.push_back(parameterDeclaration);
}


void aast::Declaration::setName(string name) {
	this->name = name;
}
void aast::Declaration::setExpression(aast::Expression* expression) {
	this->expression = expression;
}