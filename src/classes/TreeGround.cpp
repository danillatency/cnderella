#include <iostream>

#include "../classHeaders/TreeGround.h"
#include "../classHeaders/Tokens.h"


using namespace std;


Program* Program::parse(TreeGround* treeGround) {
	ClassDeclarations* classDeclarations = ClassDeclarations::parse(treeGround);
	return new Program(classDeclarations);
}
Program::Program(ClassDeclarations* classDeclarations) : classDeclarations(classDeclarations) {}


ClassDeclarations* ClassDeclarations::parse(TreeGround* treeGround) {
	ClassDeclaration* classDeclaration = ClassDeclaration::parse(treeGround);
	if (treeGround->enoughTokensForClassDeclaration()) {
		ClassDeclarations* classDeclarations = ClassDeclarations::parse(treeGround);
		return new ClassDeclarations(classDeclaration, classDeclarations);
	}
	return new ClassDeclarations(classDeclaration);
}
ClassDeclarations::ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations) : classDeclaration(classDeclaration), classDeclarations(classDeclarations) {}
ClassDeclarations::ClassDeclarations(ClassDeclaration* classDeclaration) : classDeclaration(classDeclaration), classDeclarations(nullptr) {}


ClassDeclaration* ClassDeclaration::parse(TreeGround* treeGround) {
	ClassHead* classHead = ClassHead::parse(treeGround);
	Body* body = Body::parse(treeGround);
	return new ClassDeclaration(classHead, body);
}
ClassDeclaration::ClassDeclaration(ClassHead* classHead, Body* body) : classHead(classHead), body(body) {}


ClassHead* ClassHead::parse(TreeGround* treeGround) {
	while (treeGround->nextToken()->code == TokenNewLine)
		continue;
	if (treeGround->currentToken()->code == TokenClass)
		if (treeGround->nextToken()->code == TokenIdentifier) {
			string classIdentifier = dynamic_cast<Identifier*>(treeGround->currentToken())->value;
			if (treeGround->nextToken()->code == TokenColon) {
				if (treeGround->nextToken()->code == TokenIdentifier)
					return new ClassHead(classIdentifier, dynamic_cast<Identifier*>(treeGround->currentToken())->value);
			}
			else {
				treeGround->back();
				return new ClassHead(classIdentifier);
			}
		}
	throw runtime_error("Failed to parse");
}
ClassHead::ClassHead(string name, string baseName) : name(name), baseName(baseName) {}
ClassHead::ClassHead(string name) : name(name) {}


Body* Body::parse(TreeGround* treeGround) {
	while (treeGround->nextToken()->code == TokenNewLine)
		continue;
	if (treeGround->currentToken()->code == TokenIs) {
		Statements* statements = Statements::parse(treeGround);
		if (treeGround->currentToken()->code == TokenEnd)
			return new Body(statements);
	}
	else if (treeGround->currentToken()->code == TokenArrow) {
		Statement* statement = Statement::parse(treeGround);
		Statements* statements = new Statements(statement);
		return new Body(statements);
	}
	throw runtime_error("Failed to parse");
}
Body::Body(Statements* statements) : statements(statements) {}


Statements* Statements::parse(TreeGround* treeGround) {
	Statement* statement = Statement::parse(treeGround);
	Statements* statements;
	if (statement->statement != nullptr) {
		Statements* statements = Statements::parse(treeGround);
		return new Statements(statement, statements);
	}
	return new Statements(statement);
}
Statements::Statements(Statement* statement) : statement(statement) {}
Statements::Statements(Statement* statement, Statements* statements) : statement(statement), statements(statements) {}


Statement* Statement::parse(TreeGround* treeGround) {
	while (treeGround->nextToken()->code == TokenNewLine)
		continue;
	switch (treeGround->currentToken()->code) {
	case TokenVar: {
		treeGround->back();
		LocalDeclaration* localDeclaration = LocalDeclaration::parse(treeGround);
		return new Statement(localDeclaration);
	}
	case TokenMethod: {
		treeGround->back();
		MethodDeclaration* methodDeclaration = MethodDeclaration::parse(treeGround);
		return new Statement(methodDeclaration);
	}
	case TokenThis: {
		treeGround->back();
		ConstructorDeclaration* constructorDeclaration = ConstructorDeclaration::parse(treeGround);
		return new Statement(constructorDeclaration);
	}
	case TokenIdentifier: {
		treeGround->back();
		int savedTokenIndex = treeGround->currentTokenIndex;
		try {
			Assignment* assignment = Assignment::parse(treeGround);
			return new Statement(assignment);
		}
		catch (const runtime_error& e) {
			treeGround->currentTokenIndex = savedTokenIndex;
			Expression* expression = Expression::parse(treeGround);
			return new Statement(expression);
		}
	}
	case TokenIf: {
		treeGround->back();
		IfStatement* ifStatement = IfStatement::parse(treeGround);
		return new Statement(ifStatement);
	}
	case TokenWhile: {
		treeGround->back();
		WhileStatement* whileStatement = WhileStatement::parse(treeGround);
		return new Statement(whileStatement);
	}
	case TokenReturn: {
		treeGround->back();
		ReturnStatement* returnStatement = ReturnStatement::parse(treeGround);
		return new Statement(returnStatement);
	}
	}
	return new Statement();
}
Statement::Statement(Statement* statement) : statement(statement) {}
Statement::Statement() : statement(nullptr) {}


LocalDeclaration* LocalDeclaration::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenVar)
		if (treeGround->nextToken()->code == TokenIdentifier) {
			string identifier = dynamic_cast<Identifier*>(treeGround->currentToken())->value;
			if (treeGround->nextToken()->code == TokenAssignment) {
				Expression* expression = Expression::parse(treeGround);
				return new LocalDeclaration(identifier, expression);
			}
		}
	throw runtime_error("Failed to parse");
}
LocalDeclaration::LocalDeclaration(string name, Expression* expression) : name(name), expression(expression) {}


Literal* Literal::parse(TreeGround* treeGround) {
	switch (treeGround->nextToken()->code) {
	case TokenInteger: {
		treeGround->back();
		IntegerLiteral* literalInteger = IntegerLiteral::parse(treeGround);
		return literalInteger;
	}
	case TokenBoolean: {
		treeGround->back();
		BooleanLiteral* literalBoolean = BooleanLiteral::parse(treeGround);
		return literalBoolean;
	}
	case TokenCharacter: {
		treeGround->back();
		CharacterLiteral* literalCharacter = CharacterLiteral::parse(treeGround);
		return literalCharacter;
	}
	case TokenFloat: {
		treeGround->back();
		FloatLiteral* literalFloat = FloatLiteral::parse(treeGround);
		return literalFloat;
	}
	}
	throw runtime_error("Failed to parse");
}


IntegerLiteral* IntegerLiteral::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenInteger)
		return new IntegerLiteral(dynamic_cast<Integer*>(treeGround->currentToken())->value);
	throw runtime_error("Failed to parse");
}
IntegerLiteral::IntegerLiteral(int value) : value(value) {}


BooleanLiteral* BooleanLiteral::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenBoolean)
		return new BooleanLiteral(dynamic_cast<Boolean*>(treeGround->currentToken())->value);
	throw runtime_error("Failed to parse");
}
BooleanLiteral::BooleanLiteral(bool value) : value(value) {}


CharacterLiteral* CharacterLiteral::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenCharacter)
		return new CharacterLiteral(dynamic_cast<Character*>(treeGround->currentToken())->value);
	throw runtime_error("Failed to parse");
}
CharacterLiteral::CharacterLiteral(char value) : value(value) {}


FloatLiteral* FloatLiteral::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenFloat)
		return new FloatLiteral(dynamic_cast<Float*>(treeGround->currentToken())->value);
	throw runtime_error("Failed to parse");
}
FloatLiteral::FloatLiteral(double value) : value(value) {}


MethodDeclaration* MethodDeclaration::parse(TreeGround* treeGround) {
	MethodHead* methodHead = MethodHead::parse(treeGround);
	Body* body = Body::parse(treeGround);
	return new MethodDeclaration(methodHead, body);
}
MethodDeclaration::MethodDeclaration(MethodHead* methodHead, Body* body) : methodHead(methodHead), body(body) {}


MethodHead* MethodHead::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenMethod)
		if (treeGround->nextToken()->code == TokenIdentifier) {
			string methodIdentifier = dynamic_cast<Identifier*>(treeGround->currentToken())->value;
			if (treeGround->nextToken()->code == TokenOpeningParenthesis) {
				ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
				if (treeGround->nextToken()->code == TokenClosingParenthesis)
					return new MethodHead(methodIdentifier, parameterDeclarations);
			}
		}
	throw runtime_error("Failed to parse");
}
MethodHead::MethodHead(string name, ParameterDeclarations* parameterDeclarations) : name(name), parameterDeclarations(parameterDeclarations) {}


ParameterDeclarations* ParameterDeclarations::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenClosingParenthesis) {
		treeGround->back();
		return new ParameterDeclarations();
	}
	treeGround->back();
	ParameterDeclaration* parameterDeclaration = ParameterDeclaration::parse(treeGround);
	if (treeGround->nextToken()->code == TokenComma) {
		ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
		return new ParameterDeclarations(parameterDeclaration, parameterDeclarations);
	}
	treeGround->back();
	return new ParameterDeclarations(parameterDeclaration);
}
ParameterDeclarations::ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations) : parameterDeclaration(parameterDeclaration), parameterDeclarations(parameterDeclarations) {}
ParameterDeclarations::ParameterDeclarations(ParameterDeclaration* parameterDeclaration) : parameterDeclaration(parameterDeclaration) {}
ParameterDeclarations::ParameterDeclarations() {}


ParameterDeclaration* ParameterDeclaration::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenIdentifier) {
		string parameterIdentifier = dynamic_cast<Identifier*>(treeGround->currentToken())->value;
		if (treeGround->nextToken()->code == TokenColon)
			if (treeGround->nextToken()->code == TokenIdentifier)
				return new ParameterDeclaration(parameterIdentifier, dynamic_cast<Identifier*>(treeGround->currentToken())->value);
	}
	throw runtime_error("Failed to parse");
}
ParameterDeclaration::ParameterDeclaration(string name, string baseName) : name(name), baseName(baseName) {}


ConstructorDeclaration* ConstructorDeclaration::parse(TreeGround* treeGround) {
	ConstructorHead* constructorHead = ConstructorHead::parse(treeGround);
	Body* body = Body::parse(treeGround);
	return new ConstructorDeclaration(constructorHead, body);
}
ConstructorDeclaration::ConstructorDeclaration(ConstructorHead* constructorHead, Body* body) : constructorHead(constructorHead), body(body) {}


ConstructorHead* ConstructorHead::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenThis)
		if (treeGround->nextToken()->code == TokenOpeningParenthesis) {
			ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
			if (treeGround->nextToken()->code == TokenClosingParenthesis)
				return new ConstructorHead(parameterDeclarations);
		}
	throw runtime_error("Failed to parse");
}
ConstructorHead::ConstructorHead(ParameterDeclarations* parameterDeclarations) : parameterDeclarations(parameterDeclarations) {}


Expressions* Expressions::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenClosingParenthesis) {
		treeGround->back();
		return new Expressions();
	}
	treeGround->back();
	Expression* expression = Expression::parse(treeGround);
	if (treeGround->nextToken()->code == TokenComma) {
		Expressions* expressions = Expressions::parse(treeGround);
		return new Expressions(expression, expressions);
	}
	treeGround->back();
	if (treeGround->currentToken()->code == TokenClosingParenthesis)
		treeGround->back();
	return new Expressions(expression);
}
Expressions::Expressions(Expression* expression, Expressions* expressions) : expression(expression), expressions(expressions) {}
Expressions::Expressions(Expression* expression) : expression(expression) {}
Expressions::Expressions() {}



Expression* Expression::parse(TreeGround* treeGround) {
	int savedTokenIndex = treeGround->currentTokenIndex;
	try {
		Literal* literal = Literal::parse(treeGround);
		if (treeGround->nextToken()->code == TokenComma) {
			Expression* expression = Expression::parse(treeGround);
			return new Expression(literal, expression);
		}
		return new Expression(literal);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = savedTokenIndex;
	}
	if (treeGround->nextToken()->code == TokenIdentifier) {
		string identifier = dynamic_cast<Identifier*>(treeGround->currentToken())->value;
		if (treeGround->nextToken()->code == TokenDot) {
			Expression* expression = Expression::parse(treeGround);
			return new Expression(identifier, expression);
		}
		else if (treeGround->currentToken()->code == TokenOpeningParenthesis) {
			Expressions* expressions = Expressions::parse(treeGround);
			if (treeGround->nextToken()->code == TokenClosingParenthesis)
				if (treeGround->nextToken()->code == TokenDot) {
					Expression* expression = Expression::parse(treeGround);
					return new Expression(identifier, expressions, expression);
				}
				else {
					treeGround->back();
					return new Expression(identifier, expressions);
				}
		}
		return new Expression(identifier);
	}
	throw runtime_error("Failed to parse");
}
Expression::Expression(Literal* leftPartLiteral, Expression* rightPartExpression) : leftPartLiteral(leftPartLiteral), rightPartExpression(rightPartExpression) {}
Expression::Expression(Literal* leftPartLiteral) : leftPartLiteral(leftPartLiteral) {}
Expression::Expression(string leftPartIdentifier) : leftPartIdentifier(leftPartIdentifier) {}
Expression::Expression(string leftPartIdentifier, Expression* rightPartExpression) : leftPartIdentifier(leftPartIdentifier), rightPartExpression(rightPartExpression) {}
Expression::Expression(string leftPartIdentifier, Expressions* callExpressions) : leftPartIdentifier(leftPartIdentifier), callExpressions(callExpressions) {}
Expression::Expression(string leftPartIdentifier, Expressions* callExpressions, Expression* rightPartExpression) : leftPartIdentifier(leftPartIdentifier), callExpressions(callExpressions), rightPartExpression(rightPartExpression) {}



Assignment* Assignment::parse(TreeGround* treeGround) {
	Expression* expression1 = Expression::parse(treeGround);
	if (treeGround->currentToken()->code == TokenAssignment) {
		Expression* expression2 = Expression::parse(treeGround);
		return new Assignment(expression1, expression2);
	}
	throw runtime_error("Failed to parse");
}
Assignment::Assignment(Expression* expression1, Expression* expression2) : expression1(expression1), expression2(expression2) {}


IfStatement* IfStatement::parse(TreeGround* treeGround) {
	if (treeGround->currentToken()->code == TokenIf) {
		Expression* expression = Expression::parse(treeGround);
		if (treeGround->currentToken()->code == TokenThen) {
			Statements* statements = Statements::parse(treeGround);
			if (treeGround->currentToken()->code == TokenEnd)
				return new IfStatement(expression, statements);
			if (treeGround->currentToken()->code == TokenElse) {
				Statements* statements2 = Statements::parse(treeGround);
				if (treeGround->currentToken()->code == TokenEnd)
					return new IfStatement(expression, statements, statements2);
			}
		}
	}
	throw runtime_error("Failed to parse");
}
IfStatement::IfStatement(Expression* condition, Statements* trueStatements) : condition(condition), trueStatemenets(trueStatemenets) {}
IfStatement::IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements) : condition(condition), trueStatemenets(trueStatemenets), falseStatements(falseStatements) {}


WhileStatement* WhileStatement::parse(TreeGround* treeGround) {
	if (treeGround->currentToken()->code == TokenWhile) {
		Expression* expression = Expression::parse(treeGround);
		if (treeGround->currentToken()->code == TokenLoop) {
			Statements* statement = Statements::parse(treeGround);
			if (treeGround->currentToken()->code == TokenEnd)
				return new WhileStatement(expression, statement);
		}
	}
	throw runtime_error("Failed to parse");
}
WhileStatement::WhileStatement(Expression* condition, Statements* statements) : condition(condition), statements(statements) {}


ReturnStatement* ReturnStatement::parse(TreeGround* treeGround) {
	if (treeGround->currentToken()->code == TokenReturn) {
		Expression* expression = Expression::parse(treeGround);
		return new ReturnStatement(expression);
	}
	throw runtime_error("Failed to parse");
}
ReturnStatement::ReturnStatement(Expression* expression) : expression(expression) {}


void printSpace(int times) {
	for (int i = 0; i < times; i++)
		cout << ' ';
}


TreeGround::TreeGround(vector<Token*>& tokens) : tokens(tokens), currentTokenIndex(-1) {}
void TreeGround::grow() {
	this->program = Program::parse(this);
	this->visualize(0, this->program);
}
bool TreeGround::outOfTokens() {
	return this->currentTokenIndex == this->tokens.size();
}
bool TreeGround::enoughTokensForClassDeclaration() {
	return this->tokens.size() - this->currentTokenIndex - 1 > 4;
}
Token* TreeGround::currentToken() {
	return this->tokens[this->currentTokenIndex];
}
Token* TreeGround::nextToken() {
	this->currentTokenIndex++;
	return this->currentToken();
}
void TreeGround::back() {
	this->currentTokenIndex--;
}
void TreeGround::visualize(long int depth, Entity* entity) {
	if (dynamic_cast<Program*>(entity) != nullptr) {
		cout << "Program\n";
		return this->visualize(4, dynamic_cast<Program*>(entity)->classDeclarations);
	}
	if (dynamic_cast<ClassDeclarations*>(entity) != nullptr) {
		this->visualize(4, dynamic_cast<ClassDeclarations*>(entity)->classDeclaration);
		if (dynamic_cast<ClassDeclarations*>(entity)->classDeclarations != nullptr)
			return this->visualize(4, dynamic_cast<ClassDeclarations*>(entity)->classDeclarations);
	}
	if (dynamic_cast<ClassDeclaration*>(entity) != nullptr) {
		printSpace(depth);
		cout << "Class_" << dynamic_cast<ClassDeclaration*>(entity)->classHead->name << "\n";
		this->visualize(depth + 4, dynamic_cast<ClassDeclaration*>(entity)->classHead);
		return this->visualize(depth + 4, dynamic_cast<ClassDeclaration*>(entity)->body);
	}
}
