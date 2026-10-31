#include "TreeVertices.h"


Program::Program(ClassDeclarations* classDeclarations) : classDeclarations(classDeclarations) {}


ClassDeclarations::ClassDeclarations(ClassDeclaration* classDeclaration, ClassDeclarations* classDeclarations) : classDeclaration(classDeclaration), classDeclarations(classDeclarations) {}
ClassDeclarations::ClassDeclarations(ClassDeclaration* classDeclaration) : classDeclaration(classDeclaration), classDeclarations(nullptr) {}


ClassDeclaration::ClassDeclaration(ClassHead* classHead, ClassBody* classBody) : classHead(classHead), classBody(classBody) {}


ClassHead::ClassHead(Id* name) : name(name), baseName(nullptr) {}
ClassHead::ClassHead(Id* name, Id* baseName) : name(name), baseName(baseName) {}


Id::Id(string identifier) : identifier(identifier) {}


ClassBody::ClassBody(ClassStatements* classStatements) : classStatements(classStatements) {}


ClassStatements::ClassStatements(ClassStatement* classStatement) : classStatement(classStatement), classStatements(nullptr) {}
ClassStatements::ClassStatements(ClassStatement* classStatement, ClassStatements* classStatements) : classStatement(classStatement), classStatements(classStatements) {}


MethodDeclaration::MethodDeclaration(MethodHead* methodHead, AnyBody* body) : methodHead(methodHead), body(body) {}


MethodHead::MethodHead(Id* name, ParameterDeclarations* parameterDeclarations) : name(name), parameterDeclarations(parameterDeclarations), returnType(nullptr) {}
MethodHead::MethodHead(Id* name, ParameterDeclarations* parameterDeclarations, Id* returnType) : name(name), parameterDeclarations(parameterDeclarations), returnType(returnType) {}


ParameterDeclarations::ParameterDeclarations() : parameterDeclaration(nullptr), parameterDeclarations(nullptr) {}
ParameterDeclarations::ParameterDeclarations(ParameterDeclaration* parameterDeclaration) : parameterDeclaration(parameterDeclaration), parameterDeclarations(nullptr) {}
ParameterDeclarations::ParameterDeclarations(ParameterDeclaration* parameterDeclaration, ParameterDeclarations* parameterDeclarations) : parameterDeclaration(parameterDeclaration), parameterDeclarations(parameterDeclarations) {}


ParameterDeclaration::ParameterDeclaration(Id* name, Id* identifier) : name(name), identifier(identifier) {}


AnyBody::AnyBody(Statements* statements) : statements(statements) {}


Statements::Statements(Statement* statement) : statement(statement), statements(nullptr) {}
Statements::Statements(Statement* statement, Statements* statements) : statement(statement), statements(statements) {}


LocalDeclaration::LocalDeclaration(Id* name, Expression* expression) : name(name), expression(expression) {}


IdBeginningExpression::IdBeginningExpression(Id* identifier) : identifier(identifier), accessesAndCalls(nullptr) {}
IdBeginningExpression::IdBeginningExpression(Id* identifier, AccessesAndCalls* accessesAndCalls) : identifier(identifier), accessesAndCalls(accessesAndCalls) {}


AccessAndFurther::AccessAndFurther(Id* identifier) : identifier(identifier), accessesAndCalls(nullptr) {}
AccessAndFurther::AccessAndFurther(Id* identifier, AccessesAndCalls* accessesAndCalls) : identifier(identifier), accessesAndCalls(accessesAndCalls) {}


CallAndFurther::CallAndFurther(Expressions* expressions) : expressions(expressions), accessesAndCalls(nullptr) {}
CallAndFurther::CallAndFurther(Expressions* expressions, AccessesAndCalls* accessesAndCalls) : expressions(expressions), accessesAndCalls(accessesAndCalls) {}


Expressions::Expressions() : expression(nullptr), expressions(nullptr) {}
Expressions::Expressions(Expression* expression) : expression(expression), expressions(nullptr) {}
Expressions::Expressions(Expression* expression, Expressions* expressions) : expression(expression), expressions(expressions) {}


ThisBeginningExpression::ThisBeginningExpression() : accessesAndCalls(nullptr) {}
ThisBeginningExpression::ThisBeginningExpression(AccessesAndCalls* accessesAndCalls) : accessesAndCalls(accessesAndCalls) {}


LiteralBeginningExpression::LiteralBeginningExpression(Literal* literal) : literal(literal), accessesAndCalls(nullptr) {}
LiteralBeginningExpression::LiteralBeginningExpression(Literal* literal, AccessesAndCalls* accessesAndCalls) : literal(literal), accessesAndCalls(accessesAndCalls) {}


LiteralInteger::LiteralInteger(int value) : value(value) {}


LiteralBoolean::LiteralBoolean(bool value) : value(value) {}


LiteralFloat::LiteralFloat(double value) : value(value) {}


LiteralCharacter::LiteralCharacter(char value) : value(value) {}


LiteralIntegerArray::LiteralIntegerArray(LiteralIntegers* literalIntegers) : literalIntegers(literalIntegers) {}


LiteralIntegers::LiteralIntegers(LiteralInteger* literalInteger) : literalInteger(literalInteger), literalIntegers(nullptr) {}
LiteralIntegers::LiteralIntegers(LiteralInteger* literalInteger, LiteralIntegers* literalIntegers) : literalInteger(literalInteger), literalIntegers(literalIntegers) {}


LiteralBooleanArray::LiteralBooleanArray(LiteralBooleans* literalBooleans) : literalBooleans(literalBooleans) {}


LiteralBooleans::LiteralBooleans(LiteralBoolean* literalBoolean) : literalBoolean(literalBoolean), literalBooleans(nullptr) {}
LiteralBooleans::LiteralBooleans(LiteralBoolean* literalBoolean, LiteralBooleans* literalBooleans) : literalBoolean(literalBoolean), literalBooleans(literalBooleans) {}


LiteralFloatArray::LiteralFloatArray(LiteralFloats* literalFloats) : literalFloats(literalFloats) {}


LiteralFloats::LiteralFloats(LiteralFloat* literalFloat) : literalFloat(literalFloat), literalFloats(nullptr) {}
LiteralFloats::LiteralFloats(LiteralFloat* literalFloat, LiteralFloats* literalBooleans) : literalFloat(literalFloat), literalFloats(literalFloats) {}


LiteralCharacterArray::LiteralCharacterArray(LiteralCharacters* literalCharacters) : literalCharacters(literalCharacters) {}


LiteralCharacters::LiteralCharacters(LiteralCharacter* literalCharacter) : literalCharacter(literalCharacter), literalCharacters(nullptr) {}
LiteralCharacters::LiteralCharacters(LiteralCharacter* literalCharacter, LiteralCharacters* literalCharacters) : literalCharacter(literalCharacter), literalCharacters(literalCharacters) {}


LiteralIntegerList::LiteralIntegerList(LiteralIntegers* literalIntegers) : literalIntegers(literalIntegers) {}


LiteralBooleanList::LiteralBooleanList(LiteralBooleans* literalBooleans) : literalBooleans(literalBooleans) {}


LiteralFloatList::LiteralFloatList(LiteralFloats* literalFloats) : literalFloats(literalFloats) {}


LiteralCharacterList::LiteralCharacterList(LiteralCharacters* literalCharacters) : literalCharacters(literalCharacters) {}


Assignment::Assignment(Expression* expression1, Expression* expression2) : expression1(expression1), expression2(expression2) {}


IfStatement::IfStatement(Expression* condition, Statements* trueStatements) : condition(condition), trueStatements(trueStatements) {}
IfStatement::IfStatement(Expression* condition, Statements* trueStatements, Statements* falseStatements) : condition(condition), trueStatements(trueStatements), falseStatements(falseStatements) {}


WhileStatement::WhileStatement(Expression* condition, Statements* statements) : condition(condition), statements(statements) {}


ReturnStatement::ReturnStatement(Expression* expression) : expression(expression) {}


ConstructorDeclaration::ConstructorDeclaration(ConstructorHead* constructorHead, AnyBody* body) : constructorHead(constructorHead), body(body) {}


ConstructorHead::ConstructorHead(ParameterDeclarations* parameterDeclarations) : parameterDeclarations(parameterDeclarations) {}


Declaration::Declaration(Id* name, Expression* expression) : name(name), expression(expression) {}
