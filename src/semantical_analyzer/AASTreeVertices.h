#ifndef treeverticesheader
#define treeverticesheader

#include <vector>

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
private:
	vector<ClassDeclaration*> classDeclarations;

public:
	void addClassDeclaration(ClassDeclaration* classDeclaration);

	void selfPrint(unsigned int depth) override;
};


class ClassDeclaration : public Entity {
private:
	string name;
	string baseName;
	ClassBody* classBody;

public:
	void setClassName(string name);

	void setClassBaseName(string baseName);

	void setClassBody(ClassBody* classBody);

	void selfPrint(unsigned int depth) override;
};


class ClassBody : public Entity {
private:
	vector<ClassStatement*> classStatements;

public:
	void addClassStatement(ClassStatement* classStatement);

	void selfPrint(unsigned int depth) override;
};


class ClassStatement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class MethodDeclaration : public ClassStatement {
private:
	string name;
	string type;
	vector<ParameterDeclaration*> parameterDeclarations;
	AnyBody* body;

public:
	void setName(string name);

	void setType(string type);

	void addParameterDeclaration(ParameterDeclaration* parameterDeclaration);

	void selfPrint(unsigned int depth) override;
};


class ParameterDeclaration : public Entity {
private:
	string name;
	string type;

public:
	void setName(string name);

	void setType(string type);

	void selfPrint(unsigned int depth) override;
};


class AnyBody : public Entity {
private:
	vector<Statement*> statements;

public:
	void addStatement(Statement* statement);

	void selfPrint(unsigned int depth) override;
};


class Statement : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class LocalDeclaration : public Statement {
private:
	string name;
	Expression* expression;

public:
	void setName(string name);

	void setExpression(Expression* expression);

	void selfPrint(unsigned int depth) override;
};


class Expression : public Statement {
protected:
	vector<AccessOrCall*> accessesAndCalls;

public:
	void addAccessOrCall(AccessOrCall* accessOrCall);

	virtual void selfPrint(unsigned int depth) = 0;
};


class IdentifierBeginningExpression : public Expression {
private:
	string identifier;

public:
	void setIdentifier(string identifier);

	void selfPrint(unsigned int depth) override;
};


class AccessOrCall : public Entity {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class Access : public AccessOrCall {
private:
	string identifier;

public:
	void setIdentifier(string identifier);

	void selfPrint(unsigned int depth) override;
};


class Call : public AccessOrCall {
private:
	vector<Expression*> expressions;

public:
	void addExpression(Expression* expression);

	void selfPrint(unsigned int depth) override;
};


class ThisBeginningExpression : public Expression {
public:
	void selfPrint(unsigned int depth) override;
};


class LiteralBeginningExpression : public Expression {
private:
	Literal* literal;

public:
	void selfPrint(unsigned int depth) override;
};


class Literal : public Expression {
public:
	virtual void selfPrint(unsigned int depth) = 0;
};


class LiteralInteger : public Literal {
private:
	int value;

public:
	void setValue(int value);

	void selfPrint(unsigned int depth) override;
};


class LiteralBoolean : public Literal {
private:
	bool value;

public:
	void setValue(bool value);

	void selfPrint(unsigned int depth) override;
};



class LiteralFloat : public Literal {
private:
	double value;

public:
	void setValue(int value);

	void selfPrint(unsigned int depth) override;
};



class LiteralCharacter : public Literal {
private:
	char value;

public:
	void setValue(int value);

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
