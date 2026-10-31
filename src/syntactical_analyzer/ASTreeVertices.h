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
	ClassHead* classHead;
	ClassBody* classBody;

	ClassDeclaration(ClassHead* classHead, ClassBody* classBody);

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


class Id : public Entity {
public:
	string identifier;

	Id(string identifier);

	void selfPrint(unsigned int depth) override;

	static Id* parse(TreeGround* treeGround);
};


class ClassBody : public Entity {
public:
	ClassStatements* classStatements;

	ClassBody(ClassStatements* classStatements);

	void selfPrint(unsigned int depth) override;

	static ClassBody* parse(TreeGround* treeGround);
};


class ClassStatements : public Entity {
public:
	ClassStatement* classStatement;
	ClassStatements* classStatements;

	ClassStatements(ClassStatement* classStatement);

	ClassStatements(ClassStatement* classStatement, ClassStatements* ClassStatements);

	void selfPrint(unsigned int depth) override;

	static ClassStatements* parse(TreeGround* treeGround);
};


class ClassStatement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static ClassStatement* parse(TreeGround* treeGround);
};


class MethodDeclaration : public ClassStatement {
public:
	MethodHead* methodHead;
	AnyBody* body;

	MethodDeclaration(MethodHead* methodHead, AnyBody* body);

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


class AnyBody : public Entity {
public:
	Statements* statements;

	AnyBody(Statements* statements);

	void selfPrint(unsigned int depth) override;

	static AnyBody* parse(TreeGround* treeGround);
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


class ThisBeginningExpression : public Expression {
public:
	AccessesAndCalls* accessesAndCalls;

	ThisBeginningExpression();

	ThisBeginningExpression(AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static ThisBeginningExpression* parse(TreeGround* treeGround);
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


class LiteralIntegerArray : public Literal {
public:
	LiteralIntegers* literalIntegers;

	LiteralIntegerArray(LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;

	static LiteralIntegerArray* parse(TreeGround* treeGround);
};


class LiteralIntegers : public Entity {
public:
	LiteralInteger* literalInteger;
	LiteralIntegers* literalIntegers;

	LiteralIntegers(LiteralInteger* literalInteger);

	LiteralIntegers(LiteralInteger* literalInteger, LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;

	static LiteralIntegers* parse(TreeGround* treeGround);
};


class LiteralBooleanArray : public Literal {
public:
	LiteralBooleans* literalBooleans;

	LiteralBooleanArray(LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;

	static LiteralBooleanArray* parse(TreeGround* treeGround);
};


class LiteralBooleans : public Entity {
public:
	LiteralBoolean* literalBoolean;
	LiteralBooleans* literalBooleans;

	LiteralBooleans(LiteralBoolean* literalBoolean);

	LiteralBooleans(LiteralBoolean* literalBoolean, LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;

	static LiteralBooleans* parse(TreeGround* treeGround);
};


class LiteralFloatArray : public Literal {
public:
	LiteralFloats* literalFloats;

	LiteralFloatArray(LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;

	static LiteralFloatArray* parse(TreeGround* treeGround);
};


class LiteralFloats : public Entity {
public:
	LiteralFloat* literalFloat;
	LiteralFloats* literalFloats;

	LiteralFloats(LiteralFloat* literalFloat);

	LiteralFloats(LiteralFloat* literalFloat, LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;

	static LiteralFloats* parse(TreeGround* treeGround);
};

class LiteralCharacterArray : public Literal {
public:
	LiteralCharacters* literalCharacters;

	LiteralCharacterArray(LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacterArray* parse(TreeGround* treeGround);
};


class LiteralCharacters : public Entity {
public:
	LiteralCharacter* literalCharacter;
	LiteralCharacters* literalCharacters;

	LiteralCharacters(LiteralCharacter* literalCharacter);

	LiteralCharacters(LiteralCharacter* literalCharacter, LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacters* parse(TreeGround* treeGround);
};


class LiteralIntegerList : public Literal {
public:
	LiteralIntegers* literalIntegers;

	LiteralIntegerList(LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;

	static LiteralIntegerList* parse(TreeGround* treeGround);
};


class LiteralBooleanList : public Literal {
public:
	LiteralBooleans* literalBooleans;

	LiteralBooleanList(LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;

	static LiteralBooleanList* parse(TreeGround* treeGround);
};


class LiteralFloatList : public Literal {
public:
	LiteralFloats* literalFloats;

	LiteralFloatList(LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;

	static LiteralFloatList* parse(TreeGround* treeGround);
};


class LiteralCharacterList : public Literal {
public:
	LiteralCharacters* literalCharacters;

	LiteralCharacterList(LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacterList* parse(TreeGround* treeGround);
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
	Statements* trueStatements;
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


class ConstructorDeclaration : public ClassStatement {
public:
	ConstructorHead* constructorHead;
	AnyBody* body;

	ConstructorDeclaration(ConstructorHead* constructorHead, AnyBody* body);

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


class Declaration : public ClassStatement {
public:
	Id* name;
	Expression* expression;

	Declaration(Id* name, Expression* expression);

	void selfPrint(unsigned int depth) override;

	static Declaration* parse(TreeGround* treeGround);
};


#endif
