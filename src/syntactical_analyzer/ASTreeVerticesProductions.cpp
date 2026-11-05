#include <iostream>
#include <stdexcept>

#include "ASTreeVertices.h"
#include "ASTreeGround.h"
#include "../lexical_analyzer/Tokens.h"
#include "../errors/NonTerminalErrors.h"


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
	catch (const parser::OutOfTokens& e) {
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::ClassDeclarationExpected(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::IdentifierExpected(errorSpan);
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
			Span errorSpan = treeGround->currentToken()->span;
			treeGround->currentTokenIndex = checkpoint;
			throw parser::EndExpected(errorSpan);
		}
		return new ClassBody(classStatements);
	}
	if (treeGround->currentToken()->code != TokenArrow) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::ArrowExpected(errorSpan);
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
	catch (const domain_error& e) {
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::UnexpectedToken(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::MethodExpected(errorSpan);
	}
	Id* methodIdentifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenOpeningParenthesis) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::OpeningParenthesisExpected(errorSpan);
	}
	ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingParenthesis) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingParenthesisExpected(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::CommaExpected(errorSpan);
	}
	ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
	return new ParameterDeclarations(parameterDeclaration, parameterDeclarations);
}


ParameterDeclaration* ParameterDeclaration::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	Id* parameterIdentifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenColon) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ColonExpected(errorSpan);
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
			Span errorSpan = treeGround->currentToken()->span;
			treeGround->currentTokenIndex = checkpoint;
			throw parser::EndExpected(errorSpan);
		}
		return new AnyBody(statements);
	}
	if (treeGround->currentToken()->code != TokenArrow) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::ArrowExpected(errorSpan);
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
	catch (const domain_error& e) {
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
		catch (const domain_error& e) {
			treeGround->currentTokenIndex = savedTokenIndex;
			return Expression::parse(treeGround);
		}
	}
}


LocalDeclaration* LocalDeclaration::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenVar) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::VarExpected(errorSpan);
	}
	Id* identifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenAssignment) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::AssignmentExpected(errorSpan);
	}
	Expression* expression = Expression::parse(treeGround);
	return new LocalDeclaration(identifier, expression);
}


Expression* Expression::parse(TreeGround* treeGround) {
	switch (treeGround->nextToken()->code) {
	case TokenIdentifier:
		treeGround->back();
		return IdBeginningExpression::parse(treeGround);
	case TokenThis:
		treeGround->back();
		return ThisBeginningExpression::parse(treeGround);
	case TokenBase:
		treeGround->back();
		return BaseBeginningExpression::parse(treeGround);
	default:
		treeGround->back();
		return LiteralBeginningExpression::parse(treeGround);
	}
}


IdBeginningExpression* IdBeginningExpression::parse(TreeGround* treeGround) {
	Id* identifier = Id::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const domain_error& e) {
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
	catch (const domain_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return CallAndFurther::parse(treeGround);
	}
}


AccessAndFurther* AccessAndFurther::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenDot) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::DotExpected(errorSpan);
	}
	Id* identifier = Id::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const domain_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new AccessAndFurther(identifier);
	}
	return new AccessAndFurther(identifier, accessesAndCalls);
}


CallAndFurther* CallAndFurther::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningParenthesis) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningParenthesisExpected(errorSpan);
	}
	Expressions* expressions = Expressions::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingParenthesis) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingParenthesisExpected(errorSpan);
	}
	checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const domain_error& e) {
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
	catch (const domain_error& e) {
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::ThisExpected(errorSpan);
	}
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const domain_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new ThisBeginningExpression();
	}
	return new ThisBeginningExpression(accessesAndCalls);
}


BaseBeginningExpression* BaseBeginningExpression::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenBase) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::BaseExpected(errorSpan);
	}
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const domain_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new BaseBeginningExpression();
	}
	return new BaseBeginningExpression(accessesAndCalls);
}


LiteralBeginningExpression* LiteralBeginningExpression::parse(TreeGround* treeGround) {
	Literal* literal = Literal::parse(treeGround);
	int checkpoint = treeGround->currentTokenIndex;
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(treeGround);
	}
	catch (const domain_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new LiteralBeginningExpression(literal);
	}
	return new LiteralBeginningExpression(literal, accessesAndCalls);
}


Literal* Literal::parse(TreeGround* treeGround) {
	switch (treeGround->nextToken()->code) {
	case TokenInteger:
		treeGround->back();
		return LiteralInteger::parse(treeGround);
	case TokenBoolean:
		treeGround->back();
		return LiteralBoolean::parse(treeGround);
	case TokenQuote:
		treeGround->back();
		return LiteralCharacter::parse(treeGround);
	case TokenFloat:
		treeGround->back();
		return LiteralFloat::parse(treeGround);
	case TokenDoubleQuote:
		treeGround->back();
		return LiteralCharacterArray::parse(treeGround);
	case TokenOpeningSquareBracket:
		switch (treeGround->nextToken()->code) {
		case TokenInteger:
			treeGround->back(2);
			return LiteralIntegerArray::parse(treeGround);
		case TokenBoolean:
			treeGround->back(2);
			return LiteralBooleanArray::parse(treeGround);
		case TokenFloat:
			treeGround->back(2);
			return LiteralFloatArray::parse(treeGround);
		case TokenQuote:
			treeGround->back(2);
			return LiteralCharacterArray::parse(treeGround);
		}
	case TokenOpeningCurlyBrace:
		switch (treeGround->nextToken()->code) {
		case TokenInteger:
			treeGround->back(2);
			return LiteralIntegerList::parse(treeGround);
		case TokenBoolean:
			treeGround->back(2);
			return LiteralBooleanList::parse(treeGround);
		case TokenFloat:
			treeGround->back(2);
			return LiteralFloatList::parse(treeGround);
		case TokenQuote:
			treeGround->back(2);
			return LiteralCharacterList::parse(treeGround);
		}
	}
	Span errorSpan = treeGround->currentToken()->span;
	treeGround->back();
	throw parser::FailedToParse(errorSpan);
}


LiteralInteger* LiteralInteger::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenInteger)
		return new LiteralInteger(dynamic_cast<Integer*>(treeGround->currentToken())->value);
	Span errorSpan = treeGround->currentToken()->span;
	treeGround->back();
	throw parser::FailedToParse(errorSpan);
}



LiteralBoolean* LiteralBoolean::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenBoolean)
		return new LiteralBoolean(dynamic_cast<Boolean*>(treeGround->currentToken())->value);
	Span errorSpan = treeGround->currentToken()->span;
	treeGround->back();
	throw parser::FailedToParse(errorSpan);
}



LiteralFloat* LiteralFloat::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenFloat)
		return new LiteralFloat(dynamic_cast<Float*>(treeGround->currentToken())->value);
	Span errorSpan = treeGround->currentToken()->span;
	treeGround->back();
	throw parser::FailedToParse(errorSpan);
}



LiteralCharacter* LiteralCharacter::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code == TokenCharacter)
		return new LiteralCharacter(dynamic_cast<Character*>(treeGround->currentToken())->value);
	if (treeGround->currentToken()->code != TokenQuote) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::QuoteExpected(errorSpan);
	}
	char character = dynamic_cast<Character*>(treeGround->nextToken())->value;
	if (treeGround->nextToken()->code != TokenQuote) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back(3);
		throw parser::QuoteExpected(errorSpan);
	}
	return new LiteralCharacter(character);
}



LiteralIntegerArray* LiteralIntegerArray::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningSquareBracket) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningSquareBracketExpected(errorSpan);
	}
	LiteralIntegers* literalIntegers = LiteralIntegers::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingSquareBracketExpected(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningSquareBracketExpected(errorSpan);
	}
	LiteralBooleans* literalBooleans = LiteralBooleans::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingSquareBracketExpected(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningSquareBracketExpected(errorSpan);
	}
	LiteralFloats* literalFloats = LiteralFloats::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingSquareBracketExpected(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningSquareBracketOrQuotesExpected(errorSpan);
	}
	LiteralCharacters* literalCharacters = LiteralCharacters::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingSquareBracket
		&& treeGround->currentToken()->code != TokenDoubleQuote) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingSquareBracketOrQuotesExpected(errorSpan);
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
	catch (const domain_error& e) {
		treeGround->currentTokenIndex = checkpoint;
		return new LiteralCharacters(literalCharacter);
	}
	return new LiteralCharacters(literalCharacter, literalCharacters);
}



LiteralIntegerList* LiteralIntegerList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningCurlyBraceExpected(errorSpan);
	}
	LiteralIntegers* literalIntegers = LiteralIntegers::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingCurlyBraceExpected(errorSpan);
	}
	return new LiteralIntegerList(literalIntegers);
}



LiteralBooleanList* LiteralBooleanList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningCurlyBraceExpected(errorSpan);
	}
	LiteralBooleans* literalBooleans = LiteralBooleans::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingCurlyBraceExpected(errorSpan);
	}
	return new LiteralBooleanList(literalBooleans);
}



LiteralFloatList* LiteralFloatList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningSquareBracketOrQuotesExpected(errorSpan);
	}
	LiteralFloats* literalFloats = LiteralFloats::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingSquareBracketOrQuotesExpected(errorSpan);
	}
	return new LiteralFloatList(literalFloats);
}



LiteralCharacterList* LiteralCharacterList::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenOpeningCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::OpeningSquareBracketOrQuotesExpected(errorSpan);
	}
	LiteralCharacters* literalCharacters = LiteralCharacters::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingSquareBracketOrQuotesExpected(errorSpan);
	}
	return new LiteralCharacterList(literalCharacters);
}



Assignment* Assignment::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	Expression* expression1 = Expression::parse(treeGround);
	if (treeGround->nextToken()->code != TokenAssignment) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::AssignmentSignExpected(errorSpan);
	}
	Expression* expression2 = Expression::parse(treeGround);
	return new Assignment(expression1, expression2);
}



IfStatement* IfStatement::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenIf) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::IfExpected(errorSpan);
	}
	Expression* expression = Expression::parse(treeGround);
	if (treeGround->nextToken()->code != TokenThen) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ThenExpected(errorSpan);
	}
	Statements* statements = Statements::parse(treeGround);
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code == TokenEnd)
		return new IfStatement(expression, statements);
	if (treeGround->currentToken()->code != TokenElse) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ElseExpected(errorSpan);
	}
	Statements* statements2 = Statements::parse(treeGround);
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code != TokenEnd) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::EndExpected(errorSpan);
	}
	return new IfStatement(expression, statements, statements2);
}



WhileStatement* WhileStatement::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenWhile) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::WhileExpected(errorSpan);
	}
	Expression* expression = Expression::parse(treeGround);
	if (treeGround->nextToken()->code != TokenLoop) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::LoopExpected(errorSpan);
	}
	Statements* statements = Statements::parse(treeGround);
	if (treeGround->nextToken()->code != TokenNewLine) {
		treeGround->back();
	};
	if (treeGround->nextToken()->code != TokenEnd) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::EndExpected(errorSpan);
	}
	return new WhileStatement(expression, statements);
}



ReturnStatement* ReturnStatement::parse(TreeGround* treeGround) {
	if (treeGround->nextToken()->code != TokenReturn) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::ReturnExpected(errorSpan);
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
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::ThisExpected(errorSpan);
	}
	if (treeGround->nextToken()->code != TokenOpeningParenthesis) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::OpeningParenthesisExpected(errorSpan);
	}
	ParameterDeclarations* parameterDeclarations = ParameterDeclarations::parse(treeGround);
	if (treeGround->nextToken()->code != TokenClosingParenthesis) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::ClosingParenthesisExpected(errorSpan);
	}
	return new ConstructorHead(parameterDeclarations);
}



Declaration* Declaration::parse(TreeGround* treeGround) {
	int checkpoint = treeGround->currentTokenIndex;
	if (treeGround->nextToken()->code != TokenVar) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->back();
		throw parser::VarExpected(errorSpan);
	}
	Id* identifier = Id::parse(treeGround);
	if (treeGround->nextToken()->code != TokenAssignment) {
		Span errorSpan = treeGround->currentToken()->span;
		treeGround->currentTokenIndex = checkpoint;
		throw parser::AssignmentExpected(errorSpan);
	}
	Expression* expression = Expression::parse(treeGround);
	return new Declaration(identifier, expression);
}
