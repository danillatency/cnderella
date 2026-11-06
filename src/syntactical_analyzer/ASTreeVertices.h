#ifndef astreeverticesheader
#define astreeverticesheader

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
class BaseBeginningExpression;
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

class SyntacticalAnalyzer;


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

	static Program* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ClassDeclarations : public Entity {
public:
	ClassDeclaration* classDeclaration;
	ClassDeclarations* classDeclarations;

	ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations);

	ClassDeclarations(ClassDeclaration* classDeclaration);

	void selfPrint(unsigned int depth) override;

	static ClassDeclarations* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ClassDeclaration : public Entity {
public:
	ClassHead* classHead;
	ClassBody* classBody;

	ClassDeclaration(ClassHead* classHead, ClassBody* classBody);

	void selfPrint(unsigned int depth) override;

	static ClassDeclaration* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ClassHead : public Entity {
public:
	Id* name;
	Id* baseName;

	ClassHead(Id* name, Id* baseName);

	ClassHead(Id* name);

	void selfPrint(unsigned int depth) override;

	static ClassHead* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Id : public Entity {
public:
	string identifier;

	Id(string identifier);

	void selfPrint(unsigned int depth) override;

	static Id* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ClassBody : public Entity {
public:
	ClassStatements* classStatements;

	ClassBody(ClassStatements* classStatements);

	void selfPrint(unsigned int depth) override;

	static ClassBody* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ClassStatements : public Entity {
public:
	ClassStatement* classStatement;
	ClassStatements* classStatements;

	ClassStatements(ClassStatement* classStatement);

	ClassStatements(ClassStatement* classStatement, ClassStatements* ClassStatements);

	void selfPrint(unsigned int depth) override;

	static ClassStatements* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ClassStatement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static ClassStatement* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class MethodDeclaration : public ClassStatement {
public:
	MethodHead* methodHead;
	AnyBody* body;

	MethodDeclaration(MethodHead* methodHead, AnyBody* body);

	void selfPrint(unsigned int depth) override;

	static MethodDeclaration* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class MethodHead : public Entity {
public:
	Id* name;
	ParameterDeclarations* parameterDeclarations;
	Id* returnType;

	MethodHead(Id* name, ParameterDeclarations* parameterDeclarations);

	MethodHead(Id* name, ParameterDeclarations* parameterDeclarations, Id* returnType);

	void selfPrint(unsigned int depth) override;

	static MethodHead* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ParameterDeclarations : public Entity {
public:
	ParameterDeclaration* parameterDeclaration;
	ParameterDeclarations* parameterDeclarations;

	ParameterDeclarations();

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration);

	ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations);

	void selfPrint(unsigned int depth) override;

	static ParameterDeclarations* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ParameterDeclaration : public Entity {
public:
	Id* name;
	Id* identifier;

	ParameterDeclaration(Id* name, Id* identifier);

	void selfPrint(unsigned int depth) override;

	static ParameterDeclaration* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class AnyBody : public Entity {
public:
	Statements* statements;

	AnyBody(Statements* statements);

	void selfPrint(unsigned int depth) override;

	static AnyBody* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Statements : public Entity {
public:
	Statement* statement;
	Statements* statements;

	Statements(Statement* statement);

	Statements(Statement* statement, Statements* statements);

	void selfPrint(unsigned int depth) override;

	static Statements* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Statement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static Statement* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LocalDeclaration : public Statement {
public:
	Id* name;
	Expression* expression;

	LocalDeclaration(Id* name, Expression* expression);

	void selfPrint(unsigned int depth) override;

	static LocalDeclaration* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Expression : public Statement {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static Expression* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class IdBeginningExpression : public Expression {
public:
	Id* identifier;
	AccessesAndCalls* accessesAndCalls;

	IdBeginningExpression(Id* identifier);

	IdBeginningExpression(Id* identifier, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static IdBeginningExpression* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class AccessesAndCalls : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static AccessesAndCalls* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class AccessAndFurther : public AccessesAndCalls {
public:
	Id* identifier;
	AccessesAndCalls* accessesAndCalls;

	AccessAndFurther(Id* identifier);

	AccessAndFurther(Id* identifier, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static AccessAndFurther* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class CallAndFurther : public AccessesAndCalls {
public:
	Expressions* expressions;
	AccessesAndCalls* accessesAndCalls;

	CallAndFurther(Expressions* expressions);

	CallAndFurther(Expressions* expressions, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static CallAndFurther* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Expressions : public Entity {
public:
	Expression* expression;
	Expressions* expressions;

	Expressions();

	Expressions(Expression* expression);

	Expressions(Expression* expression, Expressions* expressions);

	void selfPrint(unsigned int depth) override;

	static Expressions* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ThisBeginningExpression : public Expression {
public:
	AccessesAndCalls* accessesAndCalls;

	ThisBeginningExpression();

	ThisBeginningExpression(AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static ThisBeginningExpression* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class BaseBeginningExpression : public Expression {
public:
	AccessesAndCalls* accessesAndCalls;

	BaseBeginningExpression();

	BaseBeginningExpression(AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static BaseBeginningExpression* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralBeginningExpression : public Expression {
public:
	Literal* literal;
	AccessesAndCalls* accessesAndCalls;

	LiteralBeginningExpression(Literal* literal);

	LiteralBeginningExpression(Literal* literal, AccessesAndCalls* accessesAndCalls);

	void selfPrint(unsigned int depth) override;

	static LiteralBeginningExpression* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Literal : public Expression {
public:
	virtual void selfPrint(unsigned int depth) = 0;

	static Literal* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralInteger : public Literal {
public:
	int value;

	LiteralInteger(int value);

	void selfPrint(unsigned int depth) override;

	static LiteralInteger* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralBoolean : public Literal {
public:
	bool value;

	LiteralBoolean(bool value);

	void selfPrint(unsigned int depth) override;

	static LiteralBoolean* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralFloat : public Literal {
public:
	double value;

	LiteralFloat(double value);

	void selfPrint(unsigned int depth) override;

	static LiteralFloat* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralCharacter : public Literal {
public:
	char value;

	LiteralCharacter(char value);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacter* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralIntegerArray : public Literal {
public:
	LiteralIntegers* literalIntegers;

	LiteralIntegerArray(LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;

	static LiteralIntegerArray* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralIntegers : public Entity {
public:
	LiteralInteger* literalInteger;
	LiteralIntegers* literalIntegers;

	LiteralIntegers(LiteralInteger* literalInteger);

	LiteralIntegers(LiteralInteger* literalInteger, LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;

	static LiteralIntegers* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralBooleanArray : public Literal {
public:
	LiteralBooleans* literalBooleans;

	LiteralBooleanArray(LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;

	static LiteralBooleanArray* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralBooleans : public Entity {
public:
	LiteralBoolean* literalBoolean;
	LiteralBooleans* literalBooleans;

	LiteralBooleans(LiteralBoolean* literalBoolean);

	LiteralBooleans(LiteralBoolean* literalBoolean, LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;

	static LiteralBooleans* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralFloatArray : public Literal {
public:
	LiteralFloats* literalFloats;

	LiteralFloatArray(LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;

	static LiteralFloatArray* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralFloats : public Entity {
public:
	LiteralFloat* literalFloat;
	LiteralFloats* literalFloats;

	LiteralFloats(LiteralFloat* literalFloat);

	LiteralFloats(LiteralFloat* literalFloat, LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;

	static LiteralFloats* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};

class LiteralCharacterArray : public Literal {
public:
	LiteralCharacters* literalCharacters;

	LiteralCharacterArray(LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacterArray* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralCharacters : public Entity {
public:
	LiteralCharacter* literalCharacter;
	LiteralCharacters* literalCharacters;

	LiteralCharacters(LiteralCharacter* literalCharacter);

	LiteralCharacters(LiteralCharacter* literalCharacter, LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacters* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralIntegerList : public Literal {
public:
	LiteralIntegers* literalIntegers;

	LiteralIntegerList(LiteralIntegers* literalIntegers);

	void selfPrint(unsigned int depth) override;

	static LiteralIntegerList* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralBooleanList : public Literal {
public:
	LiteralBooleans* literalBooleans;

	LiteralBooleanList(LiteralBooleans* literalBooleans);

	void selfPrint(unsigned int depth) override;

	static LiteralBooleanList* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralFloatList : public Literal {
public:
	LiteralFloats* literalFloats;

	LiteralFloatList(LiteralFloats* literalFloats);

	void selfPrint(unsigned int depth) override;

	static LiteralFloatList* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class LiteralCharacterList : public Literal {
public:
	LiteralCharacters* literalCharacters;

	LiteralCharacterList(LiteralCharacters* literalCharacters);

	void selfPrint(unsigned int depth) override;

	static LiteralCharacterList* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Assignment : public Statement {
public:
	Expression* expression1;
	Expression* expression2;

	Assignment(Expression* expression1, Expression* expression2);

	void selfPrint(unsigned int depth) override;

	static Assignment* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class IfStatement : public Statement {
public:
	Expression* condition;
	Statements* trueStatements;
	Statements* falseStatements;

	IfStatement(Expression* condition, Statements* trueStatements);

	IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements);

	void selfPrint(unsigned int depth) override;

	static IfStatement* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class WhileStatement : public Statement {
public:
	Expression* condition;
	Statements* statements;

	WhileStatement(Expression* condition, Statements* statements);

	void selfPrint(unsigned int depth) override;

	static WhileStatement* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ReturnStatement : public Statement {
public:
	Expression* expression;

	ReturnStatement(Expression* expression);

	void selfPrint(unsigned int depth) override;

	static ReturnStatement* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ConstructorDeclaration : public ClassStatement {
public:
	ConstructorHead* constructorHead;
	AnyBody* body;

	ConstructorDeclaration(ConstructorHead* constructorHead, AnyBody* body);

	void selfPrint(unsigned int depth) override;

	static ConstructorDeclaration* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class ConstructorHead : public Entity {
public:
	ParameterDeclarations* parameterDeclarations;

	ConstructorHead(ParameterDeclarations* parameterDeclarations);

	void selfPrint(unsigned int depth) override;

	static ConstructorHead* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


class Declaration : public ClassStatement {
public:
	Id* name;
	Expression* expression;

	Declaration(Id* name, Expression* expression);

	void selfPrint(unsigned int depth) override;

	static Declaration* parse(SyntacticalAnalyzer* syntacticalAnalyzer);
};


#endif
