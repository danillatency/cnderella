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


class Entity {
public:
	virtual ~Entity() = default;
};


class Program : public Entity {
public:
	ClassDeclarations* classDeclarations;

	Program(ClassDeclarations* classDeclarations);

	static Program* parse(TreeGround* treeGround);
};


class ClassDeclarations : public Entity {
public:
	ClassDeclaration* classDeclaration;
	ClassDeclarations* classDeclarations;

	ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations);

	ClassDeclarations(ClassDeclaration* classDeclaration);

	static ClassDeclarations* parse(TreeGround* treeGround);
};


class ClassDeclaration : public Entity {
public:
	ClassHead* classHead;
	Body* body;

	ClassDeclaration(ClassHead* classHead, Body* body);

	static ClassDeclaration* parse(TreeGround* treeGround);
};


class ClassHead : public Entity {
public:
	string name;
	string baseName;

	ClassHead(string name, string baseName);
	ClassHead(string name);

	static ClassHead* parse(TreeGround* treeGround);
};


class Body : public Entity {
public:
	Statements* statements;

	Body(Statements* statements);

	static Body* parse(TreeGround* treeGround);
};


class Statements : public Entity {
public:
	Statement* statement;
	Statements* statements;

	Statements(Statement* statement);

	Statements(Statement* statement, Statements* statements);

	static Statements* parse(TreeGround* treeGround);
};


class Statement : public Entity {
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


class MethodDeclaration : public Statement {
public:
	MethodHead* methodHead;
	Body* body;

	MethodDeclaration(MethodHead* methodHead, Body* body);

	static MethodDeclaration* parse(TreeGround* treeGround);
};


class MethodHead : public Entity {
public:
	string name;
	ParameterDeclarations* parameterDeclarations;

	MethodHead(string name, ParameterDeclarations* parameterDeclarations);

	static MethodHead* parse(TreeGround* treeGround);
};


class ParameterDeclarations : public Entity {
public:
	ParameterDeclaration* parameterDeclaration;
	ParameterDeclarations* parameterDeclarations;

	ParameterDeclarations();

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration);

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations);

	static ParameterDeclarations* parse(TreeGround* treeGround);
};


class ParameterDeclaration : public Entity {
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


class ConstructorHead : public Entity {
public:
	ParameterDeclarations* parameterDeclarations;

	ConstructorHead(ParameterDeclarations* parameterDeclarations);

	static ConstructorHead* parse(TreeGround* treeGround);
};


class Expressions : public Entity {
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
	Expression* expression;

	Expression(Expression* expression);

	static Expression* parse(TreeGround* treeGround);
};


class Identifier : public Expression {
public:
	string identifier;

	Identifier(string identifier);

	static Identifier* parse(TreeGround* treeGround);
};


class Literal : public Expression {
public:
	Literal* literal;

	Literal(Literal* literal);

	static Literal* parse(TreeGround* treeGround);
};


class LiteralInteger : public Literal {
public:
	int value;

	LiteralInteger(int value);

	static LiteralInteger* parse(TreeGround* treeGround);
};


class LiteralBoolean : public Literal {
public:
	bool value;

	LiteralBoolean(bool value);

	static LiteralBoolean* parse(TreeGround* treeGround);
};


class LiteralFloat : public Literal {
public:
	double value;

	LiteralFloat(double value);

	static LiteralFloat* parse(TreeGround* treeGround);
};


class LiteralCharacter : public Literal {
public:
	char value;

	LiteralCharacter(char value);

	static LiteralCharacter* parse(TreeGround* treeGround);
};


class Access : public Expression {
public:
	Access* access;

	Access(Access* access);

	static Access* parse(TreeGround* treeGround);
};


class IdentifierAccess : public Access {
public:
	Identifier* identifier;
	Expression* expression;

	IdentifierAccess(Identifier* identifier, Expression* expression);

	static IdentifierAccess* parse(TreeGround* treeGround);
};


class CallResultAccess : public Access {
public:
	Call* call;
	Expression* expression;

	CallResultAccess(Call* call, Expression* expression);

	static CallResultAccess* parse(TreeGround* treeGround);
};


class Call : public Expression {
public:
	Expression* expression;
	Expressions* expressions;

	Call(Expression* expression, Expressions* expressions);

	static Call* parse(TreeGround* treeGround);
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

	void visualize(long int depth, Entity* entity);
};


void printSpace(int times);


#endif
