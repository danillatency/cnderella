#ifndef treegroundheader
#define treegroundheader

#include <vector>
#include <string>

#include "Tokens.h"


using namespace std;


class ClassDeclarations;
class ClassDeclaration;
class TreeGround;
class ClassHead;
class Body;
class Statements;
class Statement;
class Expressions;
class Expression;
class LocalDeclaration;
class MethodHead;
class ConstructorHead;
class ParameterDeclarations;
class ParameterDeclaration;


class Program {
public:
	ClassDeclarations* classDeclarations;

	Program(ClassDeclarations* classDeclarations);

	static Program* parse(TreeGround* treeGround);
};


class ClassDeclarations {
public:
	ClassDeclaration* classDeclaration;
	ClassDeclarations* classDeclarations;

	ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations);

	ClassDeclarations(ClassDeclaration* classDeclaration);

	static ClassDeclarations* parse(TreeGround* treeGround);
};


class ClassDeclaration {
public:
	ClassHead* classHead;
	Body* body;

	ClassDeclaration(ClassHead* classHead, Body* body);

	static ClassDeclaration* parse(TreeGround* treeGround);
};


class ClassHead {
public:
	string name;
	string baseName;

	ClassHead(string name, string baseName);
	ClassHead(string name);

	static ClassHead* parse(TreeGround* treeGround);
};


class Body {
public:
	Statements* statements;

	Body(Statements* statements);

	static Body* parse(TreeGround* treeGround);
};


class Statements {
public:
	Statement* statement;
	Statements* statements;

	Statements(Statement* statement);

	Statements(Statement* statement, Statements* statements);

	static Statements* parse(TreeGround* treeGround);
};


class Statement {
public:
	Statement* statement;

	Statement();

	Statement(Statement* statement);

	static Statement* parse(TreeGround* treeGround);
};


class LocalDeclaration : public Statement {
public:
	string name;
	Expression* expression;

	LocalDeclaration(string name, Expression* expression);

	static LocalDeclaration* parse(TreeGround* treeGround);
};


class Literal : public Statement {
public:
	static Literal* parse(TreeGround* treeGround);
};


class IntegerLiteral : public Literal {
public:
	int value;

	IntegerLiteral(int value);

	static IntegerLiteral* parse(TreeGround* treeGround);
};


class BooleanLiteral : public Literal {
public:
	bool value;

	BooleanLiteral(bool value);

	static BooleanLiteral* parse(TreeGround* treeGround);
};


class FloatLiteral : public Literal {
public:
	double value;

	FloatLiteral(double value);

	static FloatLiteral* parse(TreeGround* treeGround);
};


class CharacterLiteral : public Literal {
public:
	char value;

	CharacterLiteral(char value);

	static CharacterLiteral* parse(TreeGround* treeGround);
};


class MethodDeclaration : public Statement {
public:
	MethodHead* methodHead;
	Body* body;

	MethodDeclaration(MethodHead* methodHead, Body* body);

	static MethodDeclaration* parse(TreeGround* treeGround);
};


class MethodHead {
public:
	string name;
	ParameterDeclarations* parameterDeclarations;

	MethodHead(string name, ParameterDeclarations* parameterDeclarations);

	static MethodHead* parse(TreeGround* treeGround);
};


class ParameterDeclarations {
public:
	ParameterDeclaration* parameterDeclaration;
	ParameterDeclarations* parameterDeclarations;

	ParameterDeclarations();

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration);

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations);

	static ParameterDeclarations* parse(TreeGround* treeGround);
};


class ParameterDeclaration {
public:
	string name;
	string baseName;

	ParameterDeclaration(string name, string baseName);

	static ParameterDeclaration* parse(TreeGround* treeGround);
};


class ConstructorDeclaration : public Statement {
public:
	ConstructorHead* constructorHead;
	Body* body;

	ConstructorDeclaration(ConstructorHead* constructorHead, Body* body);

	static ConstructorDeclaration* parse(TreeGround* treeGround);
};


class ConstructorHead {
public:
	ParameterDeclarations* parameterDeclarations;

	ConstructorHead(ParameterDeclarations* parameterDeclarations);

	static ConstructorHead* parse(TreeGround* treeGround);
};


class Expressions {
public:
	Expression* expression;
	Expressions* expressions;

	Expressions();

	Expressions(Expression* expression);

	Expressions(Expression* expression, Expressions* expressions);

	static Expressions* parse(TreeGround* treeGround);
};


class Expression : public Statement {
public:
	Literal* leftPartLiteral;
	string leftPartIdentifier;
	Expressions* callExpressions;
	Expression* rightPartExpression;

	Expression(Literal* leftPartLiteral, Expression* rightPartExpression);

	Expression(Literal* leftPartLiteral);

	Expression(string leftPartIdentifier, Expression* rightPartExpression);

	Expression(string leftPartIdentifier, Expressions* callExpressions);

	Expression(string leftPartIdentifier, Expressions* callExpressions, Expression* rightPartExpression);

	Expression(string leftPartIdentifier);

	static Expression* parse(TreeGround* treeGround);
};


class Assignment : public Statement {
public:
	Expression* expression1;
	Expression* expression2;

	Assignment(Expression* expression1, Expression* expression2);

	static Assignment* parse(TreeGround* treeGround);
};


class IfStatement : public Statement {
public:
	Expression* condition;
	Statements* trueStatemenets;
	Statements* falseStatements;

	IfStatement(Expression* condition, Statements* trueStatements);

	IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements);

	static IfStatement* parse(TreeGround* treeGround);
};


class WhileStatement : public Statement {
public:
	Expression* condition;
	Statements* statements;

	WhileStatement(Expression* condition, Statements* statements);

	static WhileStatement* parse(TreeGround* treeGround);
};


class ReturnStatement : public Statement {
public:
	Expression* expression;

	ReturnStatement(Expression* expression);

	static ReturnStatement* parse(TreeGround* treeGround);
};


class TreeGround {
private:
	Program* program;
	vector<Token*>& tokens;

public:
	long int currentTokenIndex;

	TreeGround(vector<Token*>& tokens);

	void grow();

	bool outOfTokens();

	Token* currentToken();

	Token* nextToken();

	void back();

	bool enoughTokensForClassDeclaration();
};


#endif
