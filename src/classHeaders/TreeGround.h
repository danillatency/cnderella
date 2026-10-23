#ifndef treegroundheader
#define treegroundheader

#include <vector>
#include <string>

#include "Tokens.h"


using namespace std;


class Program;
class ClassDeclarations;
class ClassDeclaration;
class ClassHead;
class Id;
class Body;
class Statements;
class Statement;
class MethodDeclaration;
class MethodHead;
class ParameterDeclarations;
class ParameterDeclaration;
class ConstructorDeclaration;
class ConstructorHead;
class LocalDeclaration;
class Expression;
class Literal;
class LiteralInteger;
class LiteralBoolean;
class LiteralFloat;
class LiteralCharacter;
class IdBeginningExpression;
class AccessesAndCalls;
class AccessAndFurther;
class CallAndFurther;
class Expressions;
class LiteralBeginningExpression;
class Assignment;
class IfStatement;
class WhileStatement;
class ReturnStatement;

class TreeGround;


class Entity {
public:
	virtual ~Entity() = default;

	virtual void selfPrint(unsigned int depth) = 0;
};


class Program : public Entity {
public:
	ClassDeclarations* classDeclarations;

	Program(ClassDeclarations* classDeclarations);

	void selfPrint(unsigned int depth) override;

	static Program* parse(TreeGround* treeGround);
};


class ClassDeclarations : public Entity {
public:
	ClassDeclaration* classDeclaration;
	ClassDeclarations* classDeclarations;

	ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations);

	ClassDeclarations(ClassDeclaration* classDeclaration);

	void selfPrint(unsigned int depth) override;

	static ClassDeclarations* parse(TreeGround* treeGround);
};


class ClassDeclaration : public Entity {
public:
	ClassHead* head;
	Body* body;

	ClassDeclaration(ClassHead* head, Body* body);

	void selfPrint(unsigned int depth) override;

	static ClassDeclaration* parse(TreeGround* treeGround);
};


class ClassHead : public Entity {
public:
	Id* name;
	Id* baseName;

	ClassHead(Id* name, Id* baseName);

	ClassHead(Id* name);

	void selfPrint(unsigned int depth) override;

	static ClassHead* parse(TreeGround* treeGround);
};


class Body : public Entity {
public:
	Statements* statements;

	Body(Statements* statements);

	void selfPrint(unsigned int depth) override;

	static Body* parse(TreeGround* treeGround);
};


class Statements : public Entity {
public:
	Statement* statement;
	Statements* statements;

	Statements(Statement* statement);

	Statements(Statement* statement, Statements* statements);

	void selfPrint(unsigned int depth) override;

	static Statements* parse(TreeGround* treeGround);
};


class Statement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static Statement* parse(TreeGround* treeGround);
};


class MethodDeclaration : public Statement {
public:
	MethodHead* methodHead;
	Body* body;

	MethodDeclaration(MethodHead* methodHead, Body* body);

	void selfPrint(unsigned int depth) override;

	static MethodDeclaration* parse(TreeGround* treeGround);
};


class MethodHead : public Entity {
public:
	Id* name;
	ParameterDeclarations* parameterDeclarations;
	Id* returnType;

	MethodHead(Id* name, ParameterDeclarations* parameterDeclarations);

	MethodHead(Id* name, ParameterDeclarations* parameterDeclarations, Id* returnType);

	void selfPrint(unsigned int depth) override;

	static MethodHead* parse(TreeGround* treeGround);
};


class ParameterDeclarations : public Entity {
public:
	ParameterDeclaration* parameterDeclaration;
	ParameterDeclarations* parameterDeclarations;

	ParameterDeclarations();

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration);

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations);

	void selfPrint(unsigned int depth) override;

	static ParameterDeclarations* parse(TreeGround* treeGround);
};


class ParameterDeclaration : public Entity {
public:
	Id* name;
	Id* identifier;

	ParameterDeclaration(Id* name, Id* identifier);

	void selfPrint(unsigned int depth) override;

	static ParameterDeclaration* parse(TreeGround* treeGround);
};


class ConstructorDeclaration : public Statement {
public:
	ConstructorHead* constructorHead;
	Body* body;

	ConstructorDeclaration(ConstructorHead* constructorHead, Body* body);

	void selfPrint(unsigned int depth) override;

	static ConstructorDeclaration* parse(TreeGround* treeGround);
};


class ConstructorHead : public Entity {
public:
	ParameterDeclarations* parameterDeclarations;

	ConstructorHead(ParameterDeclarations* parameterDeclarations);

	void selfPrint(unsigned int depth) override;

	static ConstructorHead* parse(TreeGround* treeGround);
};


class LocalDeclaration : public Statement {
public:
	Id* name;
	Expression* expression;

	LocalDeclaration(Id* name, Expression* expression);

	void selfPrint(unsigned int depth) override;

	static LocalDeclaration* parse(TreeGround* treeGround);
};


class Expression : public Statement {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static Expression* parse(TreeGround* treeGround);
};


class Id : public Expression {
public:
	string identifier;

	Id(string identifier);

	void selfPrint(unsigned int depth) override;

	static Id* parse(TreeGround* treeGround);
};


class Literal : public Expression {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static Literal* parse(TreeGround* treeGround);
};


class LiteralInteger : public Literal {
public:
	int value;

	LiteralInteger(int value);

	void selfPrint(unsigned int depth) override;

	static LiteralInteger* parse(TreeGround* treeGround);
};


class LiteralBoolean : public Literal {
public:
	bool value;

	LiteralBoolean(bool value);

	void selfPrint(unsigned int depth) override;

	static LiteralBoolean* parse(TreeGround* treeGround);
};


class LiteralFloat : public Literal {
public:
	double value;

	LiteralFloat(double value);

	void selfPrint(unsigned int depth) override;

	static LiteralFloat* parse(TreeGround* treeGround);
};


class LiteralCharacter : public Literal {
public:
	char value;

	LiteralCharacter(char value);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacter* parse(TreeGround* treeGround);
};


class IdBeginningExpression : public Expression {
public:
	Id* identifier;
	AccessesAndCalls* accessesAndCalls;

	IdBeginningExpression(Id* identifier);

	IdBeginningExpression(Id* identifier, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static IdBeginningExpression* parse(TreeGround* treeGround);
};


class AccessesAndCalls : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static AccessesAndCalls* parse(TreeGround* treeGround);
};


class AccessAndFurther : public AccessesAndCalls {
public:
	Id* identifier;
	AccessesAndCalls* accessesAndCalls;

	AccessAndFurther(Id* identifier);

	AccessAndFurther(Id* identifier, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static AccessAndFurther* parse(TreeGround* treeGround);
};


class CallAndFurther : public AccessesAndCalls {
public:
	Expressions* expressions;
	AccessesAndCalls* accessesAndCalls;

	CallAndFurther(Expressions* expressions);

	CallAndFurther(Expressions* expressions, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static CallAndFurther* parse(TreeGround* treeGround);
};


class Expressions : public Entity {
public:
	Expression* expression;
	Expressions* expressions;

	Expressions();

	Expressions(Expression* expression);

	Expressions(Expression* expression, Expressions* expressions);

	void selfPrint(unsigned int depth) override;

	static Expressions* parse(TreeGround* treeGround);
};


class LiteralBeginningExpression : public Expression {
public:
	Literal* literal;
	AccessesAndCalls* accessesAndCalls;

	LiteralBeginningExpression(Literal* literal);

	LiteralBeginningExpression(Literal* literal, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static LiteralBeginningExpression* parse(TreeGround* treeGround);
};


class Assignment : public Statement {
public:
	Expression* expression1;
	Expression* expression2;

	Assignment(Expression* expression1, Expression* expression2);

	void selfPrint(unsigned int depth) override;

	static Assignment* parse(TreeGround* treeGround);
};


class IfStatement : public Statement {
public:
	Expression* condition;
	Statements* trueStatemenets;
	Statements* falseStatements;

	IfStatement(Expression* condition, Statements* trueStatements);

	IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements);

	void selfPrint(unsigned int depth) override;

	static IfStatement* parse(TreeGround* treeGround);
};


class WhileStatement : public Statement {
public:
	Expression* condition;
	Statements* statements;

	WhileStatement(Expression* condition, Statements* statements);

	void selfPrint(unsigned int depth) override;

	static WhileStatement* parse(TreeGround* treeGround);
};


class ReturnStatement : public Statement {
public:
	Expression* expression;

	ReturnStatement(Expression* expression);

	void selfPrint(unsigned int depth) override;

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

	string currentTokenStringValue();

	Token* nextToken();

	void back();

	bool enoughTokensForClassDeclaration();

	void visualize(unsigned int depth, Entity* entity);
};


#endif
