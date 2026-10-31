#include <iostream>
#include <stdexcept>

#include "TreeVertices.h"
#include "TreeGround.h"
#include "../lexical_analyzer/Tokens.h"


Program* Program::parse(TreeGround* treeGround) {
	ClassDeclarations* classDeclarations = ClassDeclarations::parse(treeGround);
	return new Program(classDeclarations);
}


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


ClassDeclaration* ClassDeclaration::parse(TreeGround* treeGround) {
	ClassHead* classHead = ClassHead::parse(treeGround);
	ClassBody* classBody = ClassBody::parse(treeGround);
	return new ClassDeclaration(classHead, classBody);
}


ClassHead* ClassHead::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code != TokenClass) {
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


Id* Id::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenIdentifier) {
		treeGround->back();
		throw runtime_error("Identifier expected");
	}
	return new Id(treeGround->currentTokenStringValue());
}


ClassBody* ClassBody::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code == TokenIs) {
		ClassStatements* classStatements = ClassStatements::parse(treeGround);
		if (treeGround->nextToken()->code != TokenNewLine) {
			treeGround->back();
		};
		if (treeGround->nextToken()->code != TokenEnd) {
			treeGround->currentTokenIndex = checkpoint;
			throw runtime_error("end expected");
		}
		return new ClassBody(classStatements);
	}
	if (treeGround->currentToken()->code != TokenArrow) {
		treeGround->back();
		throw runtime_error("arrow expected");
	}
	ClassStatement* classStatement = ClassStatement::parse(treeGround);
	return new ClassBody(new ClassStatements(classStatement));
}


ClassStatements* ClassStatements::parse(TreeGround* treeGround) {
	ClassStatement* classStatement = ClassStatement::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	ClassStatements* classStatements;
	try {
		classStatements = ClassStatements::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new ClassStatements(classStatement);
	}
	return new ClassStatements(classStatement, classStatements);
}


ClassStatement* ClassStatement::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	switch (treeGround->nextToken()->code) {
	case TokenVar:
		treeGround->back();
		return Declaration::parse(treeGround);
	case TokenMethod:
		treeGround->back();
		return MethodDeclaration::parse(treeGround);
	case TokenThis:
		treeGround->back();
		return ConstructorDeclaration::parse(treeGround);
	default:
		treeGround->back();
		throw runtime_error("unexpected token");
	}
}


MethodDeclaration* MethodDeclaration::parse(TreeGround* treeGround) {
	MethodHead* methodHead = MethodHead::parse(treeGround);
	AnyBody* body = AnyBody::parse(treeGround);
	return new MethodDeclaration(methodHead, body);
}


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
	if (treeGround->nextToken()->code == TokenColon) {
		Id* returnType = Id::parse(treeGround);
		return new MethodHead(methodIdentifier, parameterDeclarations, returnType);
	}
	treeGround->back();
	return new MethodHead(methodIdentifier, parameterDeclarations);
}


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


AnyBody* AnyBody::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code == TokenIs) {
		Statements* statements = Statements::parse(treeGround);
		if (treeGround->nextToken()->code != TokenNewLine) {
			treeGround->back();
		};
		if (treeGround->nextToken()->code != TokenEnd) {
			treeGround->currentTokenIndex = checkpoint;
			throw runtime_error("end expected");
		}
		return new AnyBody(statements);
	}
	if (treeGround->currentToken()->code != TokenArrow) {
		treeGround->back();
		throw runtime_error("arrow expected");
	}
	Statement* statement = Statement::parse(treeGround);
	return new AnyBody(new Statements(statement));
}


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


Statement* Statement::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	switch (treeGround->nextToken()->code) {
	case TokenVar:
		treeGround->back();
		return LocalDeclaration::parse(treeGround);
	case TokenIf:
		treeGround->back();
		return IfStatement::parse(treeGround);
	case TokenWhile:
		treeGround->back();
		return WhileStatement::parse(treeGround);
	case TokenReturn:
		treeGround->back();
		return ReturnStatement::parse(treeGround);
	default:
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
}


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


Expression* Expression::parse(TreeGround* treeGround) {
	switch (treeGround->nextToken()->code) {
	case TokenIdentifier: {
		treeGround->back();
		return IdBeginningExpression::parse(treeGround);
	}
	case TokenThis: {
		treeGround->back();
		return ThisBeginningExpression::parse(treeGround);
	}
	default: {
		treeGround->back();
		return LiteralBeginningExpression::parse(treeGround);
	}
	}
}


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


ThisBeginningExpression* ThisBeginningExpression::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenThis) {
		treeGround->back();
		throw runtime_error("this expected");
	}
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new ThisBeginningExpression();
	}
	return new ThisBeginningExpression(accessesAndCalls);
}


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
	case TokenQuote: {
		treeGround->back();
		return LiteralCharacter::parse(treeGround);
	}
	case TokenFloat: {
		treeGround->back();
		return LiteralFloat::parse(treeGround);
	}
	case TokenDoubleQuote: {
		treeGround->back();
		return LiteralCharacterArray::parse(treeGround);
	}
	case TokenOpeningSquareBracket: {
		switch (treeGround->nextToken()->code) {
		case TokenInteger: {
			treeGround->back(2);
			return LiteralIntegerArray::parse(treeGround);
		}
		case TokenBoolean: {
			treeGround->back(2);
			return LiteralBooleanArray::parse(treeGround);
		}
		case TokenFloat: {
			treeGround->back(2);
			return LiteralFloatArray::parse(treeGround);
		}
		case TokenQuote: {
			treeGround->back(2);
			return LiteralCharacterArray::parse(treeGround);
		}
		}
	}
	case TokenOpeningCurlyBrace: {
		switch (treeGround->nextToken()->code) {
		case TokenInteger: {
			treeGround->back(2);
			return LiteralIntegerList::parse(treeGround);
		}
		case TokenBoolean: {
			treeGround->back(2);
			return LiteralBooleanList::parse(treeGround);
		}
		case TokenFloat: {
			treeGround->back(2);
			return LiteralFloatList::parse(treeGround);
		}
		case TokenQuote: {
			treeGround->back(2);
			return LiteralCharacterList::parse(treeGround);
		}
		}
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



LiteralBoolean* LiteralBoolean::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenBoolean)
		return new LiteralBoolean(dynamic_cast<Boolean*>(treeGround->currentToken())->value);
	treeGround->back();
	throw runtime_error("Failed to parse");
}



LiteralFloat* LiteralFloat::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenFloat)
		return new LiteralFloat(dynamic_cast<Float*>(treeGround->currentToken())->value);
	treeGround->back();
	throw runtime_error("Failed to parse");
}



LiteralCharacter* LiteralCharacter::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenCharacter)
		return new LiteralCharacter(dynamic_cast<Character*>(treeGround->currentToken())->value);
	if (treeGround->currentToken()->code != TokenQuote) {
		treeGround->back();
		throw runtime_error("' expected");
	}
	char character = dynamic_cast<Character*>(treeGround->nextToken())->value;
	if (treeGround->nextToken()->code != TokenQuote) {
		treeGround->back(3);
		throw runtime_error("' expected");
	}
	return new LiteralCharacter(character);
}



LiteralIntegerArray* LiteralIntegerArray::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningSquareBracket) {
		treeGround->back();
		throw runtime_error("[ expected");
	}
	LiteralIntegers* literalIntegers = LiteralIntegers::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("] expected");
	}
	return new LiteralIntegerArray(literalIntegers);
}



LiteralIntegers* LiteralIntegers::parse(TreeGround* treeGround) {
	LiteralInteger* integer = LiteralInteger::parse(treeGround);
	if (treeGround->nextToken()->code != TokenComma) {
		treeGround->back();
		return new LiteralIntegers(integer);
	}
	LiteralIntegers* integers = LiteralIntegers::parse(treeGround);
	return new LiteralIntegers(integer, integers);
}



LiteralBooleanArray* LiteralBooleanArray::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningSquareBracket) {
		treeGround->back();
		throw runtime_error("[ expected");
	}
	LiteralBooleans* literalBooleans = LiteralBooleans::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("] expected");
	}
	return new LiteralBooleanArray(literalBooleans);
}



LiteralBooleans* LiteralBooleans::parse(TreeGround* treeGround) {
	LiteralBoolean* boolean = LiteralBoolean::parse(treeGround);
	if (treeGround->nextToken()->code != TokenComma) {
		treeGround->back();
		return new LiteralBooleans(boolean);
	}
	LiteralBooleans* booleans = LiteralBooleans::parse(treeGround);
	return new LiteralBooleans(boolean, booleans);
}



LiteralFloatArray* LiteralFloatArray::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningSquareBracket) {
		treeGround->back();
		throw runtime_error("[ expected");
	}
	LiteralFloats* literalFloats = LiteralFloats::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("] expected");
	}
	return new LiteralFloatArray(literalFloats);
}



LiteralFloats* LiteralFloats::parse(TreeGround* treeGround) {
	LiteralFloat* literalFloat = LiteralFloat::parse(treeGround);
	if (treeGround->nextToken()->code != TokenComma) {
		treeGround->back();
		return new LiteralFloats(literalFloat);
	}
	LiteralFloats* literalFloats = LiteralFloats::parse(treeGround);
	return new LiteralFloats(literalFloat, literalFloats);
}



LiteralCharacterArray* LiteralCharacterArray::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningSquareBracket 
		&& treeGround->currentToken()->code != TokenDoubleQuote) {
		treeGround->back();
		throw runtime_error("[ or \" expected");
	}
	LiteralCharacters* literalCharacters = LiteralCharacters::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket
		&& treeGround->currentToken()->code != TokenDoubleQuote) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("] or \" expected");
	}
	return new LiteralCharacterArray(literalCharacters);
}



LiteralCharacters* LiteralCharacters::parse(TreeGround* treeGround) {
	LiteralCharacter* literalCharacter = LiteralCharacter::parse(treeGround);
	if (treeGround->nextToken()->code != TokenComma)
		treeGround->back();
	int checkpoint = treeGround->currentTokenIndex;
	LiteralCharacters* literalCharacters;
	try {
		literalCharacters = LiteralCharacters::parse(treeGround);
	}
	catch (const runtime_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new LiteralCharacters(literalCharacter);
	}
	return new LiteralCharacters(literalCharacter, literalCharacters);
}



LiteralIntegerList* LiteralIntegerList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		treeGround->back();
		throw runtime_error("{ expected");
	}
	LiteralIntegers* literalIntegers = LiteralIntegers::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("} expected");
	}
	return new LiteralIntegerList(literalIntegers);
}



LiteralBooleanList* LiteralBooleanList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		treeGround->back();
		throw runtime_error("{ expected");
	}
	LiteralBooleans* literalBooleans = LiteralBooleans::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("} expected");
	}
	return new LiteralBooleanList(literalBooleans);
}



LiteralFloatList* LiteralFloatList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		treeGround->back();
		throw runtime_error("[ expected");
	}
	LiteralFloats* literalFloats = LiteralFloats::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("] expected");
	}
	return new LiteralFloatList(literalFloats);
}



LiteralCharacterList* LiteralCharacterList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		treeGround->back();
		throw runtime_error("[ expected");
	}
	LiteralCharacters* literalCharacters = LiteralCharacters::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("] expected");
	}
	return new LiteralCharacterList(literalCharacters);
}



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
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code == TokenEnd)
		return new IfStatement(expression, statements);
	if (treeGround->currentToken()->code != TokenElse) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("else expected");
	}
	Statements* statements2 = Statements::parse(treeGround);
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code != TokenEnd) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("end expected");
	}
	return new IfStatement(expression, statements, statements2);
}



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
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code != TokenEnd) {
		treeGround->currentTokenIndex = checkpoint;
		throw runtime_error("end expected");
	}
	return new WhileStatement(expression, statements);
}



ReturnStatement* ReturnStatement::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenReturn) {
		treeGround->back();
		throw runtime_error("return expected");
	}
	Expression* expression = Expression::parse(treeGround);
	return new ReturnStatement(expression);
}



ConstructorDeclaration* ConstructorDeclaration::parse(TreeGround* treeGround) {
	ConstructorHead* constructorHead = ConstructorHead::parse(treeGround);
	AnyBody* body = AnyBody::parse(treeGround);
	return new ConstructorDeclaration(constructorHead, body);
}



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



Declaration* Declaration::parse(TreeGround* treeGround) {
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
	return new Declaration(identifier, expression);
}
