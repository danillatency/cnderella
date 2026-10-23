#include <iostream>
#include <stdexcept>

#include "../classHeaders/TreeGround.h"
#include "../classHeaders/Tokens.h"


using namespace std;


int printPadding = 4;


void printSpace(unsigned int times) {
	for (int i = 0; i != times; i++)
		cout << ' ';
}


Program* Program::parse(TreeGround* treeGround) {
	ClassDeclarations* classDeclarations = ClassDeclarations::parse(treeGround);
	return new Program(classDeclarations);
}
void Program::selfPrint(unsigned int depth) {
	this->classDeclarations->selfPrint(depth);
	cout << "\n";
}
Program::Program(ClassDeclarations* classDeclarations) : classDeclarations(classDeclarations) {}


ClassDeclarations* ClassDeclarations::parse(TreeGround* treeGround) {
	ClassDeclaration* classDeclaration = ClassDeclaration::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	ClassDeclarations* classDeclarations;
	try {
		classDeclarations = ClassDeclarations::parse(treeGround);
	} 
	catch (const exception& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new ClassDeclarations(classDeclaration);
	}
	return new ClassDeclarations(classDeclaration, classDeclarations);
}
void ClassDeclarations::selfPrint(unsigned int depth) {
	this->classDeclaration->selfPrint(depth);
	if (this->classDeclarations) {
		cout << "\n";
		this->classDeclarations->selfPrint(depth);
	}
}
ClassDeclarations::ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations) : classDeclaration(classDeclaration), classDeclarations(classDeclarations) {}
ClassDeclarations::ClassDeclarations(ClassDeclaration* classDeclaration) : classDeclaration(classDeclaration), classDeclarations(nullptr) {}


ClassDeclaration* ClassDeclaration::parse(TreeGround* treeGround) {
	ClassHead* classHead = ClassHead::parse(treeGround);
	Body* body = Body::parse(treeGround);
	return new ClassDeclaration(classHead, body);
}
void ClassDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[class]\n";
	this->head->selfPrint(depth + printPadding);
	this->body->selfPrint(depth + printPadding);
}
ClassDeclaration::ClassDeclaration(ClassHead* classHead, Body* body) : head(classHead), body(body) {}


ClassHead* ClassHead::parse(TreeGround* treeGround) {
	while (treeGround->nextToken()->code == TokenNewLine)
		continue;
	if (treeGround->currentToken()->code != TokenClass) {
		treeGround->back();
		throw runtime_error("class expected");
	}
	Id* classIdentifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code == TokenColon) {
		Id* baseIdentifier = Id::parse(treeGround);
		return new ClassHead(classIdentifier, baseIdentifier);
	}
	treeGround->back();
	return new ClassHead(classIdentifier);
}
void ClassHead::selfPrint(unsigned int depth) {
	this->name->selfPrint(depth);
	if (this->baseName) {
		cout << " : ";
		this->baseName->selfPrint(0);
	}
	cout << "\n";
}
ClassHead::ClassHead(Id* name, Id* baseName) : name(name), baseName(baseName) {}
ClassHead::ClassHead(Id* name) : name(name), baseName(nullptr) {}


Id* Id::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenIdentifier) {
		treeGround->back();
		throw runtime_error("Identifier expected");
	}
	return new Id(treeGround->currentTokenStringValue());
}
void Id::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->identifier;
}
Id::Id(string identifier) : identifier(identifier) {}


Body* Body::parse(TreeGround* treeGround) {
	while (treeGround->nextToken()->code == TokenNewLine)
		continue;
	if (treeGround->currentToken()->code == TokenIs) {
		Statements* statements = Statements::parse(treeGround);
		while (treeGround->nextToken()->code == TokenNewLine)
			continue;
		if (treeGround->currentToken()->code != TokenEnd) {
			treeGround->back();
			throw runtime_error("end expected");
		}
		return new Body(statements);
	}
	if (treeGround->currentToken()->code != TokenArrow) {
		treeGround->back();
		throw runtime_error("arrow expected");
	}
	Statement* statement = Statement::parse(treeGround);
	return new Body(new Statements(statement));
}
void Body::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[body]\n";
	this->statements->selfPrint(depth + printPadding);
}
Body::Body(Statements* statements) : statements(statements) {}


Statements* Statements::parse(TreeGround* treeGround) {
	Statement* statement = Statement::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	Statements* statements;
	try {
		statements = Statements::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new Statements(statement);
	}
	return new Statements(statement, statements);
}
void Statements::selfPrint(unsigned int depth) {
	this->statement->selfPrint(depth);
	if (this->statements)
		this->statements->selfPrint(depth);
}
Statements::Statements(Statement* statement) : statement(statement), statements(nullptr) {}
Statements::Statements(Statement* statement, Statements* statements) : statement(statement), statements(statements) {}


Statement* Statement::parse(TreeGround* treeGround) {
	while (treeGround->nextToken()->code == TokenNewLine)
		continue;
	switch (treeGround->currentToken()->code) {
	case TokenVar: {
		treeGround->back();
		return LocalDeclaration::parse(treeGround);
	}
	case TokenMethod: {
		treeGround->back();
		return MethodDeclaration::parse(treeGround);
	}
	case TokenThis: {
		treeGround->back();
		return ConstructorDeclaration::parse(treeGround);
	}
	case TokenIdentifier: {
		treeGround->back();
		int savedTokenIndex = treeGround->currentTokenIndex;
		try {
			return Assignment::parse(treeGround);
		}
		catch (const runtime_error& e) {
			treeGround->currentTokenIndex = savedTokenIndex;
			return Expression::parse(treeGround);
		}
	}
	case TokenIf: {
		treeGround->back();
		return IfStatement::parse(treeGround);
	}
	case TokenWhile: {
		treeGround->back();
		return WhileStatement::parse(treeGround);
	}
	case TokenReturn: {
		treeGround->back();
		return ReturnStatement::parse(treeGround);
	}
	}
	treeGround->back();
	throw runtime_error("statement expected");
}


MethodDeclaration* MethodDeclaration::parse(TreeGround* treeGround) {
	MethodHead* methodHead = MethodHead::parse(treeGround);
	Body* body = Body::parse(treeGround);
	return new MethodDeclaration(methodHead, body);
}
void MethodDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[method]\n";
	this->methodHead->selfPrint(depth + printPadding);
	this->body->selfPrint(depth + printPadding);
}
MethodDeclaration::MethodDeclaration(MethodHead* methodHead, Body* body) : methodHead(methodHead), body(body) {}


MethodHead* MethodHead::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenMethod) {
		treeGround->back();
		throw runtime_error("method expected");
	}
	Id* methodIdentifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenOpeningParenthesis) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("opening parenthesis expected");
	}
	ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingParenthesis) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("closing parenthesis expected");
	}
	return new MethodHead(methodIdentifier, parameterDeclarations);
}
void MethodHead::selfPrint(unsigned int depth) {
	this->name->selfPrint(depth);
	cout << "\n[parameters]\n";
	this->parameterDeclarations->selfPrint(depth + printPadding);
}
MethodHead::MethodHead(Id* name, ParameterDeclarations* parameterDeclarations) : name(name), parameterDeclarations(parameterDeclarations) {}


ParameterDeclarations* ParameterDeclarations::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code == TokenClosingParenthesis) {
		treeGround->back();
		return new ParameterDeclarations();
	}
	treeGround->back();
	ParameterDeclaration* parameterDeclaration = ParameterDeclaration::parse(treeGround);
	if (treeGround->nextToken()->code == TokenClosingParenthesis) {
		treeGround->back();
		return new ParameterDeclarations(parameterDeclaration);
	}
	treeGround->back();
	if (treeGround->nextToken()->code != TokenComma) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("comma expected");
	}
	ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
	return new ParameterDeclarations(parameterDeclaration, parameterDeclarations);
}
void ParameterDeclarations::selfPrint(unsigned int depth) {
	if (this->parameterDeclaration)
		this->parameterDeclaration->selfPrint(depth);
	if (this->parameterDeclarations)
		this->parameterDeclarations->selfPrint(depth);
}
ParameterDeclarations::ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations) : parameterDeclaration(parameterDeclaration), parameterDeclarations(parameterDeclarations) {}
ParameterDeclarations::ParameterDeclarations(ParameterDeclaration* parameterDeclaration) : parameterDeclaration(parameterDeclaration), parameterDeclarations(nullptr) {}
ParameterDeclarations::ParameterDeclarations() : parameterDeclaration(nullptr), parameterDeclarations(nullptr) {}


ParameterDeclaration* ParameterDeclaration::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	Id* parameterIdentifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenColon) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("colon expected");
	}
	Id* typeIdentifier = Id::parse(treeGround);
	return new ParameterDeclaration(parameterIdentifier, typeIdentifier);
}
void ParameterDeclaration::selfPrint(unsigned int depth) {
	this->name->selfPrint(depth);
	cout << " : ";
	this->identifier->selfPrint(0);
	cout << "\n";
}
ParameterDeclaration::ParameterDeclaration(Id* name, Id* identifier) : name(name), identifier(identifier) {}


ConstructorDeclaration* ConstructorDeclaration::parse(TreeGround* treeGround) {
	ConstructorHead* constructorHead = ConstructorHead::parse(treeGround);
	Body* body = Body::parse(treeGround);
	return new ConstructorDeclaration(constructorHead, body);
}
void ConstructorDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[constructor]\n";
	this->constructorHead->selfPrint(depth + printPadding);
	this->body->selfPrint(depth + printPadding);
}
ConstructorDeclaration::ConstructorDeclaration(ConstructorHead* constructorHead, Body* body) : constructorHead(constructorHead), body(body) {}


ConstructorHead* ConstructorHead::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenThis) {
		treeGround->back();
		throw runtime_error("this expected");
	}
	if (treeGround->nextToken()->code != TokenOpeningParenthesis) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("opening parenthesis expected");
	}
	ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingParenthesis) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("closing parenthesis expected");
	}
	return new ConstructorHead(parameterDeclarations);
}
void ConstructorHead::selfPrint(unsigned int depth) {
	this->parameterDeclarations->selfPrint(depth);
}
ConstructorHead::ConstructorHead(ParameterDeclarations* parameterDeclarations) : parameterDeclarations(parameterDeclarations) {}


LocalDeclaration* LocalDeclaration::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenVar) {
		treeGround->back();
		throw runtime_error("var expected");
	}
	Id* identifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenAssignment) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("assignment expected");
	}
	Expression* expression = Expression::parse(treeGround);
	return new LocalDeclaration(identifier, expression);
}
void LocalDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[declaration]\n";
	this->name->selfPrint(depth + printPadding);
	cout << "\n";
	this->expression->selfPrint(depth + printPadding);
}
LocalDeclaration::LocalDeclaration(Id* name, Expression* expression) : name(name), expression(expression) {}


Expression* Expression::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenIdentifier) {
		treeGround->back();
		return IdBeginningExpression::parse(treeGround);
	}
	treeGround->back();
	return LiteralBeginningExpression::parse(treeGround);
}


Literal* Literal::parse(TreeGround* treeGround) {
	switch (treeGround->nextToken()->code) {
	case TokenInteger: {
		treeGround->back();
		return LiteralInteger::parse(treeGround);
	}
	case TokenBoolean: {
		treeGround->back();
		return LiteralBoolean::parse(treeGround);
	}
	case TokenCharacter: {
		treeGround->back();
		return LiteralCharacter::parse(treeGround);
	}
	case TokenFloat: {
		treeGround->back();
		return LiteralFloat::parse(treeGround);
	}
	}
	treeGround->back();
	throw runtime_error("Failed to parse");
}


LiteralInteger* LiteralInteger::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenInteger)
		return new LiteralInteger(dynamic_cast<Integer*>(treeGround->currentToken())->value);
	treeGround->back();
	throw runtime_error("Failed to parse");
}
void LiteralInteger::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}
LiteralInteger::LiteralInteger(int value) : value(value) {}


LiteralBoolean* LiteralBoolean::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenBoolean)
		return new LiteralBoolean(dynamic_cast<Boolean*>(treeGround->currentToken())->value);
	treeGround->back();
	throw runtime_error("Failed to parse");
}
void LiteralBoolean::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}
LiteralBoolean::LiteralBoolean(bool value) : value(value) {}


LiteralFloat* LiteralFloat::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenFloat)
		return new LiteralFloat(dynamic_cast<Float*>(treeGround->currentToken())->value);
	treeGround->back();
	throw runtime_error("Failed to parse");
}
void LiteralFloat::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}
LiteralFloat::LiteralFloat(double value) : value(value) {}


LiteralCharacter* LiteralCharacter::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenCharacter)
		return new LiteralCharacter(dynamic_cast<Character*>(treeGround->currentToken())->value);
	treeGround->back();
	throw runtime_error("Failed to parse");
}
void LiteralCharacter::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}
LiteralCharacter::LiteralCharacter(char value) : value(value) {}


IdBeginningExpression* IdBeginningExpression::parse(TreeGround* treeGround) {
	Id* identifier = Id::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new IdBeginningExpression(identifier);
	}
	return new IdBeginningExpression(identifier, accessesAndCalls);
}
void IdBeginningExpression::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[expression]\n";
	this->identifier->selfPrint(depth + printPadding);
	cout << "\n";
	if (this->accessesAndCalls)
		this->accessesAndCalls->selfPrint(depth + printPadding);
}
IdBeginningExpression::IdBeginningExpression(Id* identifier) : identifier(identifier), accessesAndCalls(nullptr) {}
IdBeginningExpression::IdBeginningExpression(Id* identifier, AccessesAndCalls* accessesAndCalls) : identifier(identifier), accessesAndCalls(accessesAndCalls) {}


AccessesAndCalls* AccessesAndCalls::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	try {
		return AccessAndFurther::parse(treeGround);
	} 
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return CallAndFurther::parse(treeGround);
	}
}


AccessAndFurther* AccessAndFurther::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenDot) {
		treeGround->back();
		throw runtime_error("Dot expected");
	}
	Id* identifier = Id::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new AccessAndFurther(identifier);
	}
	return new AccessAndFurther(identifier, accessesAndCalls);
}
void AccessAndFurther::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[access] ";
	this->identifier->selfPrint(0);
	if (this->accessesAndCalls) {
		cout << "\n";
		this->accessesAndCalls->selfPrint(depth + printPadding);
	}
}
AccessAndFurther::AccessAndFurther(Id* identifier) : identifier(identifier), accessesAndCalls(nullptr) {}
AccessAndFurther::AccessAndFurther(Id* identifier, AccessesAndCalls* accessesAndCalls) : identifier(identifier), accessesAndCalls(accessesAndCalls) {}


CallAndFurther* CallAndFurther::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningParenthesis) {
		treeGround->back();
		throw runtime_error("Opening parenthesis expected");
	}
	Expressions* expressions = Expressions::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingParenthesis) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("Closing parenthesis expected");
	}
	checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new CallAndFurther(expressions);
	}
	return new CallAndFurther(expressions, accessesAndCalls);
}
void CallAndFurther::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[call with]\n";
	this->expressions->selfPrint(depth + printPadding);
	if (this->accessesAndCalls)
		this->accessesAndCalls->selfPrint(depth + printPadding);
}
CallAndFurther::CallAndFurther(Expressions* expressions) : expressions(expressions), accessesAndCalls(nullptr) {}
CallAndFurther::CallAndFurther(Expressions* expressions, AccessesAndCalls* accessesAndCalls) : expressions(expressions), accessesAndCalls(accessesAndCalls) {}


Expressions* Expressions::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	Expression* expression;
	try {
		expression = Expression::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new Expressions();
	}
	if (treeGround->nextToken()->code != TokenComma) {
		treeGround->back();
		return new Expressions(expression);
	}
	Expressions* expressions = Expressions::parse(treeGround);
	return new Expressions(expression, expressions);
}
void Expressions::selfPrint(unsigned int depth) {
	if (this->expression)
		this->expression->selfPrint(depth);
	if (this->expressions) {
		printSpace(depth);
		cout << ", ";
		this->expressions->selfPrint(depth);
	}
}
Expressions::Expressions() : expression(nullptr), expressions(nullptr) {}
Expressions::Expressions(Expression* expression) : expression(expression), expressions(nullptr) {}
Expressions::Expressions(Expression* expression, Expressions* expressions) : expression(expression), expressions(expressions) {}


LiteralBeginningExpression* LiteralBeginningExpression::parse(TreeGround* treeGround) {
	Literal* literal = Literal::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new LiteralBeginningExpression(literal);
	}
	return new LiteralBeginningExpression(literal, accessesAndCalls);
}
void LiteralBeginningExpression::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[expression]\n";
	this->literal->selfPrint(depth + printPadding);
	cout << "\n";
	if (this->accessesAndCalls)
		this->accessesAndCalls->selfPrint(depth + printPadding);
}
LiteralBeginningExpression::LiteralBeginningExpression(Literal* literal) : literal(literal), accessesAndCalls(nullptr) {}
LiteralBeginningExpression::LiteralBeginningExpression(Literal* literal, AccessesAndCalls* accessesAndCalls) : literal(literal), accessesAndCalls(accessesAndCalls) {}


Assignment* Assignment::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	Expression* expression1 = Expression::parse(treeGround);
	if (treeGround->nextToken()->code != TokenAssignment) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error(":= expected");
	}
	Expression* expression2 = Expression::parse(treeGround);
	return new Assignment(expression1, expression2);
}
void Assignment::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[assign]\n";
	this->expression1->selfPrint(depth + printPadding);
	printSpace(depth);
	cout << "to";
	this->expression2->selfPrint(depth + printPadding);
}
Assignment::Assignment(Expression* expression1, Expression* expression2) : expression1(expression1), expression2(expression2) {}


IfStatement* IfStatement::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenIf) {
		treeGround->back();
		throw runtime_error("if expected");
	}
	Expression* expression = Expression::parse(treeGround);
	if (treeGround->nextToken()->code != TokenThen) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("then expected");
	}
	Statements* statements = Statements::parse(treeGround);
	if (treeGround->nextToken()->code == TokenEnd)
		return new IfStatement(expression, statements);
	if (treeGround->currentToken()->code != TokenElse) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("else expected");
	}
	Statements* statements2 = Statements::parse(treeGround);
	if (treeGround->nextToken()->code != TokenEnd) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("end expected");
	}
	return new IfStatement(expression, statements, statements2);
}
void IfStatement::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[if]\n";
	this->condition->selfPrint(depth + printPadding);
	printSpace(depth);
	cout << "[then]\n";
	this->trueStatemenets->selfPrint(depth + printPadding);
	if (this->falseStatements); {
		printSpace(depth);
		cout << "[else]\n";
		this->falseStatements->selfPrint(depth + printPadding);
	}
}
IfStatement::IfStatement(Expression* condition, Statements* trueStatements) : condition(condition), trueStatemenets(trueStatemenets) {}
IfStatement::IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements) : condition(condition), trueStatemenets(trueStatemenets), falseStatements(falseStatements) {}


WhileStatement* WhileStatement::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenWhile) {
		treeGround->back();
		throw runtime_error("while expected");
	}
	Expression* expression = Expression::parse(treeGround);
	if (treeGround->nextToken()->code != TokenLoop) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("loop expected");
	}
	Statements* statements = Statements::parse(treeGround);
	if (treeGround->nextToken()->code != TokenEnd) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("end expected");
	}
	return new WhileStatement(expression, statements);
}
void WhileStatement::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[while]\n";
	this->condition->selfPrint(depth + printPadding);
	printSpace(depth);
	cout << "[loop]\n";
	this->statements->selfPrint(depth + printPadding);
}
WhileStatement::WhileStatement(Expression* condition, Statements* statements) : condition(condition), statements(statements) {}


ReturnStatement* ReturnStatement::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenReturn) {
		treeGround->back();
		throw runtime_error("return expected");
	}
	Expression* expression = Expression::parse(treeGround);
	return new ReturnStatement(expression);
}
void ReturnStatement::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[return]\n";
	this->expression->selfPrint(depth + printPadding);
}
ReturnStatement::ReturnStatement(Expression* expression) : expression(expression) {}


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
	return this->tokens.at(this->currentTokenIndex);
}
string TreeGround::currentTokenStringValue() {
	return dynamic_cast<Identifier*>(this->currentToken())->value;
}
Token* TreeGround::nextToken() {
	return this->tokens.at(++this->currentTokenIndex);
}
void TreeGround::back() {
	this->currentTokenIndex--;
}
void TreeGround::visualize(unsigned int depth, Entity* entity) {
	entity->selfPrint(depth);
}
