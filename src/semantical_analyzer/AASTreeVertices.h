#ifndef treeverticesheader
#define treeverticesheader

#include "../lexical_analyzer/Tokens.h"


class Program;
class ClassDeclarations;
class ClassDeclaration;
class ClassHead;
class Id;
class ClassBody;
class ClassStatements;
class ClassStatement;
class MethodDeclaration;
class MethodHead;
class ParameterDeclarations;
class ParameterDeclaration;
class AnyBody;
class Statements;
class Statement;
class LocalDeclaration;
class Expression;
class IdBeginningExpression;
class AccessesAndCalls;
class AccessAndFurther;
class CallAndFurther;
class Expressions;
class ThisBeginningExpression;
class LiteralBeginningExpression;

class Literal;
class LiteralInteger;
class LiteralBoolean;
class LiteralFloat;
class LiteralCharacter;
class LiteralIntegerArray;
class LiteralIntegers;
class LiteralBooleanArray;
class LiteralBooleans;
class LiteralFloatArray;
class LiteralFloats;
class LiteralCharacterArray;
class LiteralCharacters;
class LiteralIntegerList;
class LiteralBooleanList;
class LiteralFloatList;
class LiteralCharacterList;

class Assignment;
class IfStatement;
class WhileStatement;
class ReturnStatement;
class ConstructorDeclaration;
class ConstructorHead;
class Declaration;

class TreeGround;


class Entity {
protected:
	int printPadding = 4;

public:
	virtual ~Entity() = default;

	virtual void selfPrint(unsigned int depth) = 0;
};


class Program : public Entity {
public:
	ClassDeclarations* classDeclarations;

	Program(ClassDeclarations* classDeclarations);

	void selfPrint(unsigned int depth) override;
};


class ClassDeclarations : public Entity {
public:
	ClassDeclaration* classDeclaration;
	ClassDeclarations* classDeclarations;

	ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations);

	ClassDeclarations(ClassDeclaration* classDeclaration);

	void selfPrint(unsigned int depth) override;
};


class ClassDeclaration : public Entity {
public:
	ClassHead* classHead;
	ClassBody* classBody;

	ClassDeclaration(ClassHead* classHead, ClassBody* classBody);

	void selfPrint(unsigned int depth) override;
};


class ClassHead : public Entity {
public:
	Id* name;
	Id* baseName;

	ClassHead(Id* name, Id* baseName);

	ClassHead(Id* name);

	void selfPrint(unsigned int depth) override;
};


class Id : public Entity {
public:
	string identifier;

	Id(string identifier);

	void selfPrint(unsigned int depth) override;
};


class ClassBody : public Entity {
public:
	ClassStatements* classStatements;

	ClassBody(ClassStatements* classStatements);

	void selfPrint(unsigned int depth) override;
};


class ClassStatements : public Entity {
public:
	ClassStatement* classStatement;
	ClassStatements* classStatements;

	ClassStatements(ClassStatement* classStatement);

	ClassStatements(ClassStatement* classStatement, ClassStatements* ClassStatements);

	void selfPrint(unsigned int depth) override;
};


class ClassStatement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class MethodDeclaration : public ClassStatement {
public:
	MethodHead* methodHead;
	AnyBody* body;

	MethodDeclaration(MethodHead* methodHead, AnyBody* body);

	void selfPrint(unsigned int depth) override;
};


class MethodHead : public Entity {
public:
	Id* name;
	ParameterDeclarations* parameterDeclarations;
	Id* returnType;

	MethodHead(Id* name, ParameterDeclarations* parameterDeclarations);

	MethodHead(Id* name, ParameterDeclarations* parameterDeclarations, Id* returnType);

	void selfPrint(unsigned int depth) override;
};


class ParameterDeclarations : public Entity {
public:
	ParameterDeclaration* parameterDeclaration;
	ParameterDeclarations* parameterDeclarations;

	ParameterDeclarations();

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration);

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations);

	void selfPrint(unsigned int depth) override;
};


class ParameterDeclaration : public Entity {
public:
	Id* name;
	Id* identifier;

	ParameterDeclaration(Id* name, Id* identifier);

	void selfPrint(unsigned int depth) override;
};


class AnyBody : public Entity {
public:
	Statements* statements;

	AnyBody(Statements* statements);

	void selfPrint(unsigned int depth) override;
};


class Statements : public Entity {
public:
	Statement* statement;
	Statements* statements;

	Statements(Statement* statement);

	Statements(Statement* statement, Statements* statements);

	void selfPrint(unsigned int depth) override;
};


class Statement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class LocalDeclaration : public Statement {
public:
	Id* name;
	Expression* expression;

	LocalDeclaration(Id* name, Expression* expression);

	void selfPrint(unsigned int depth) override;
};


class Expression : public Statement {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class IdBeginningExpression : public Expression {
public:
	Id* identifier;
	AccessesAndCalls* accessesAndCalls;

	IdBeginningExpression(Id* identifier);

	IdBeginningExpression(Id* identifier, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;
};


class AccessesAndCalls : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class AccessAndFurther : public AccessesAndCalls {
public:
	Id* identifier;
	AccessesAndCalls* accessesAndCalls;

	AccessAndFurther(Id* identifier);

	AccessAndFurther(Id* identifier, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;
};


class CallAndFurther : public AccessesAndCalls {
public:
	Expressions* expressions;
	AccessesAndCalls* accessesAndCalls;

	CallAndFurther(Expressions* expressions);

	CallAndFurther(Expressions* expressions, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;
};


class Expressions : public Entity {
public:
	Expression* expression;
	Expressions* expressions;

	Expressions();

	Expressions(Expression* expression);

	Expressions(Expression* expression, Expressions* expressions);

	void selfPrint(unsigned int depth) override;
};


class ThisBeginningExpression : public Expression {
public:
	AccessesAndCalls* accessesAndCalls;

	ThisBeginningExpression();

	ThisBeginningExpression(AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;
};


class LiteralBeginningExpression : public Expression {
public:
	Literal* literal;
	AccessesAndCalls* accessesAndCalls;

	LiteralBeginningExpression(Literal* literal);

	LiteralBeginningExpression(Literal* literal, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;
};


class Literal : public Expression {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class LiteralInteger : public Literal {
public:
	int value;

	LiteralInteger(int value);

	void selfPrint(unsigned int depth) override;
};


class LiteralBoolean : public Literal {
public:
	bool value;

	LiteralBoolean(bool value);

	void selfPrint(unsigned int depth) override;
};


class LiteralFloat : public Literal {
public:
	double value;

	LiteralFloat(double value);

	void selfPrint(unsigned int depth) override;
};


class LiteralCharacter : public Literal {
public:
	char value;

	LiteralCharacter(char value);

	void selfPrint(unsigned int depth) override;
};


class LiteralIntegerArray : public Literal {
public:
	LiteralIntegers* literalIntegers;

	LiteralIntegerArray(LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;
};


class LiteralIntegers : public Entity {
public:
	LiteralInteger* literalInteger;
	LiteralIntegers* literalIntegers;

	LiteralIntegers(LiteralInteger* literalInteger);

	LiteralIntegers(LiteralInteger* literalInteger, LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;
};


class LiteralBooleanArray : public Literal {
public:
	LiteralBooleans* literalBooleans;

	LiteralBooleanArray(LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;
};


class LiteralBooleans : public Entity {
public:
	LiteralBoolean* literalBoolean;
	LiteralBooleans* literalBooleans;

	LiteralBooleans(LiteralBoolean* literalBoolean);

	LiteralBooleans(LiteralBoolean* literalBoolean, LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;
};


class LiteralFloatArray : public Literal {
public:
	LiteralFloats* literalFloats;

	LiteralFloatArray(LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;
};


class LiteralFloats : public Entity {
public:
	LiteralFloat* literalFloat;
	LiteralFloats* literalFloats;

	LiteralFloats(LiteralFloat* literalFloat);

	LiteralFloats(LiteralFloat* literalFloat, LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;
};

class LiteralCharacterArray : public Literal {
public:
	LiteralCharacters* literalCharacters;

	LiteralCharacterArray(LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;
};


class LiteralCharacters : public Entity {
public:
	LiteralCharacter* literalCharacter;
	LiteralCharacters* literalCharacters;

	LiteralCharacters(LiteralCharacter* literalCharacter);

	LiteralCharacters(LiteralCharacter* literalCharacter, LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;
};


class LiteralIntegerList : public Literal {
public:
	LiteralIntegers* literalIntegers;

	LiteralIntegerList(LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;
};


class LiteralBooleanList : public Literal {
public:
	LiteralBooleans* literalBooleans;

	LiteralBooleanList(LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;
};


class LiteralFloatList : public Literal {
public:
	LiteralFloats* literalFloats;

	LiteralFloatList(LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;
};


class LiteralCharacterList : public Literal {
public:
	LiteralCharacters* literalCharacters;

	LiteralCharacterList(LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;
};


class Assignment : public Statement {
public:
	Expression* expression1;
	Expression* expression2;

	Assignment(Expression* expression1, Expression* expression2);

	void selfPrint(unsigned int depth) override;
};


class IfStatement : public Statement {
public:
	Expression* condition;
	Statements* trueStatements;
	Statements* falseStatements;

	IfStatement(Expression* condition, Statements* trueStatements);

	IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements);

	void selfPrint(unsigned int depth) override;
};


class WhileStatement : public Statement {
public:
	Expression* condition;
	Statements* statements;

	WhileStatement(Expression* condition, Statements* statements);

	void selfPrint(unsigned int depth) override;
};


class ReturnStatement : public Statement {
public:
	Expression* expression;

	ReturnStatement(Expression* expression);

	void selfPrint(unsigned int depth) override;
};


class ConstructorDeclaration : public ClassStatement {
public:
	ConstructorHead* constructorHead;
	AnyBody* body;

	ConstructorDeclaration(ConstructorHead* constructorHead, AnyBody* body);

	void selfPrint(unsigned int depth) override;
};


class ConstructorHead : public Entity {
public:
	ParameterDeclarations* parameterDeclarations;

	ConstructorHead(ParameterDeclarations* parameterDeclarations);

	void selfPrint(unsigned int depth) override;
};


class Declaration : public ClassStatement {
public:
	Id* name;
	Expression* expression;

	Declaration(Id* name, Expression* expression);

	void selfPrint(unsigned int depth) override;
};


#endif
