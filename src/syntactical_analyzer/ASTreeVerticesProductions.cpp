#include <iostream>
#include <stdexcept>

#include "ASTreeVertices.h"
#include "SyntacticalAnalyzer.h"
#include "../lexical_analyzer/Tokens.h"
#include "../errors/NonTerminalErrors.h"


// All parsing functions guarantee same syntactical analyzer token pointer after an unsuccessful parsing attempt;
// For that purpose, every non-leading ::parse() call is wrapped with try-catch (sometimes "int checkpoint" is introduced to wayback an undeterminable pointer)
// Also, ::parse() calls can be wrapped with try-catch to build errors traceback (with RuleParseError)
Program* Program::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	ClassDeclarations* classDeclarations = ClassDeclarations::parse(syntacticalAnalyzer);
	return new Program(classDeclarations);
}


ClassDeclarations* ClassDeclarations::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	ClassDeclaration* classDeclaration;
	try {
		classDeclaration = ClassDeclaration::parse(syntacticalAnalyzer);
	}
	catch (const parser::OutOfTokens& e) {
		return new ClassDeclarations();
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("class declaration", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	ClassDeclarations* classDeclarations;
	try {
		classDeclarations = ClassDeclarations::parse(syntacticalAnalyzer);
	} 
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new ClassDeclarations(classDeclaration, classDeclarations);
}


ClassDeclaration* ClassDeclaration::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	syntacticalAnalyzer->skipPossibleNewLines();
	ClassHead* classHead;
	try {
		classHead = ClassHead::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("class head", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	ClassBody* classBody;
	try {
		classBody = ClassBody::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("class body", errorSpan, e.what());
	}
	return new ClassDeclaration(classHead, classBody);
}


ClassHead* ClassHead::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenClass) {
		syntacticalAnalyzer->back();
		throw parser::ClassDeclarationExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	Id* classIdentifier;
	try {
		classIdentifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw parser::RuleParseError("identifier", syntacticalAnalyzer->relativeToken(2)->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code == TokenColon) {
		Id* baseIdentifier;
		try {
			baseIdentifier = Id::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("identifier", errorSpan, e.what());
		}
		return new ClassHead(classIdentifier, baseIdentifier);
	}
	syntacticalAnalyzer->back();
	return new ClassHead(classIdentifier);
}


Id* Id::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	if (syntacticalAnalyzer->nextToken()->code != TokenIdentifier) {
		syntacticalAnalyzer->back();
		throw parser::IdentifierExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	return new Id(syntacticalAnalyzer->currentTokenStringValue());
}


ClassBody* ClassBody::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	syntacticalAnalyzer->skipPossibleNewLines();
	if (syntacticalAnalyzer->nextToken()->code == TokenIs) {
		ClassStatements* classStatements;
		try {
			classStatements = ClassStatements::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			syntacticalAnalyzer->setToken(checkpoint);
			throw e;
		}
		syntacticalAnalyzer->skipPossibleNewLines();
		if (syntacticalAnalyzer->nextToken()->code != TokenEnd) {
			Span errorSpan = syntacticalAnalyzer->currentToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::EndExpected("class body", errorSpan);
		}
		return new ClassBody(classStatements);
	}
	if (syntacticalAnalyzer->currentToken()->code != TokenArrow) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ArrowExpected("class body", errorSpan);
	}
	ClassStatement* classStatement;
	try {
		classStatement = ClassStatement::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("class statement", errorSpan, e.what());
	}
	return new ClassBody(new ClassStatements(classStatement));
}


ClassStatements* ClassStatements::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	ClassStatement* classStatement;
	try {
		classStatement = ClassStatement::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new ClassStatements();
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("class statement", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	ClassStatements* classStatements;
	try {
		classStatements = ClassStatements::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new ClassStatements(classStatement, classStatements);
}


ClassStatement* ClassStatement::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	syntacticalAnalyzer->skipPossibleNewLines();
	switch (syntacticalAnalyzer->nextToken()->code) {
	case TokenVar:
		syntacticalAnalyzer->back();
		try {
			return Declaration::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("class declaration", errorSpan, e.what());
		}
	case TokenMethod:
		syntacticalAnalyzer->back();
		try {
			return MethodDeclaration::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("method declaration", errorSpan, e.what());
		}
	case TokenThis:
		syntacticalAnalyzer->back();
		try {
			return ConstructorDeclaration::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("constructor declaration", errorSpan, e.what());
		}
	case TokenEnd: {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::EnumerationEnd("class statements", errorSpan);
	}
	default:
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::VarMethodOrThisExpected(errorSpan);
	}
}


MethodDeclaration* MethodDeclaration::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	MethodHead* methodHead;
	try {
		methodHead = MethodHead::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("method head", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	AnyBody* body;
	try {
		body = AnyBody::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("method body", errorSpan, e.what());
	}
	return new MethodDeclaration(methodHead, body);
}


MethodHead* MethodHead::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenMethod) {
		syntacticalAnalyzer->back();
		throw parser::MethodExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	Id* methodIdentifier;
	try {
		methodIdentifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("method identifier", errorSpan, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningParenthesis) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::OpeningParenthesisExpected(errorSpan);
	}
	ParameterDeclarations* parameterDeclarations;
	try {
		parameterDeclarations = ParameterDeclarations::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingParenthesis) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingParenthesisExpected("method head", errorSpan);
	}
	if (syntacticalAnalyzer->nextToken()->code == TokenColon) {
		Id* returnType;
		try {
			returnType = Id::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("return identifier", errorSpan, e.what());
		}
		return new MethodHead(methodIdentifier, parameterDeclarations, returnType);
	}
	syntacticalAnalyzer->back();
	return new MethodHead(methodIdentifier, parameterDeclarations);
}


ParameterDeclarations* ParameterDeclarations::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	ParameterDeclaration* parameterDeclaration;
	try {
		parameterDeclaration = ParameterDeclaration::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new ParameterDeclarations();
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("parameter declaration", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenComma) {
		syntacticalAnalyzer->back();
		return new ParameterDeclarations(parameterDeclaration);
	}
	ParameterDeclarations* parameterDeclarations;
	try {
		parameterDeclarations = ParameterDeclarations::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new ParameterDeclarations(parameterDeclaration, parameterDeclarations);
}


ParameterDeclaration* ParameterDeclaration::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	// separate empty case
	if (syntacticalAnalyzer->nextToken()->code == TokenClosingParenthesis) {
		syntacticalAnalyzer->back();
		throw parser::EnumerationEnd("parameter declarations", syntacticalAnalyzer->relativeToken()->span);
	}
	syntacticalAnalyzer->back();
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	Id* parameterIdentifier;
	try {
		parameterIdentifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("parameter identifier", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenColon) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ColonExpected(errorSpan);
	}
	Id* typeIdentifier;
	try {
		typeIdentifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("parameter type identifier", errorSpan, e.what());
	}
	return new ParameterDeclaration(parameterIdentifier, typeIdentifier);
}


AnyBody* AnyBody::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	syntacticalAnalyzer->skipPossibleNewLines();
	if (syntacticalAnalyzer->nextToken()->code == TokenIs) {
		Statements* statements;
		try {
			statements = Statements::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			syntacticalAnalyzer->setToken(checkpoint);
			throw e;
		}
		syntacticalAnalyzer->skipPossibleNewLines();
		if (syntacticalAnalyzer->nextToken()->code != TokenEnd) {
			Span errorSpan = syntacticalAnalyzer->currentToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::EndExpected("body", errorSpan);
		}
		return new AnyBody(statements);
	}
	if (syntacticalAnalyzer->currentToken()->code != TokenArrow) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ArrowExpected("body", errorSpan);
	}
	Statement* statement;
	try {
		statement = Statement::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("body statement", errorSpan, e.what());
	}
	return new AnyBody(new Statements(statement));
}


Statements* Statements::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	Statement* statement;
	try {
		statement = Statement::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new Statements();
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("statement", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	Statements* statements;
	try {
		statements = Statements::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new Statements(statement, statements);
}


Statement* Statement::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	syntacticalAnalyzer->skipPossibleNewLines();
	switch (syntacticalAnalyzer->nextToken()->code) {
	case TokenVar:
		syntacticalAnalyzer->back();
		try {
			return LocalDeclaration::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("local declaration", errorSpan, e.what());
		}
	case TokenIf:
		syntacticalAnalyzer->back();
		try {
			return IfStatement::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("if statement", errorSpan, e.what());
		}
	case TokenWhile:
		syntacticalAnalyzer->back();
		try {
			return WhileStatement::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("while statement", errorSpan, e.what());
		}
	case TokenReturn:
		syntacticalAnalyzer->back();
		try {
			return ReturnStatement::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
			syntacticalAnalyzer->setToken(checkpoint);
			throw parser::RuleParseError("return statement", errorSpan, e.what());
		}
	case TokenIdentifier:
		syntacticalAnalyzer->back();
		try {
			return Assignment::parse(syntacticalAnalyzer);
		}
		catch (const domain_error& e) {
			try {
				return Expression::parse(syntacticalAnalyzer);
			}
			catch (const domain_error& e) {
				Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
				syntacticalAnalyzer->setToken(checkpoint);
				throw parser::RuleParseError("expression", errorSpan, e.what());
			}
		}
	case TokenEnd: {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::EnumerationEnd("statements", errorSpan);
	}
	default:
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::VarIfWhileReturnIdentifier(errorSpan);
	}
}


LocalDeclaration* LocalDeclaration::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenVar) {
		syntacticalAnalyzer->back();
		throw parser::VarExpected("local declaration", syntacticalAnalyzer->relativeToken()->span);
	}
	Id* identifier;
	try {
		identifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("identifiier", errorSpan, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenAssignment) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::AssignmentSignExpected("local declaration", errorSpan);
	}
	Expression* expression;
	try {
		expression = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("expression", errorSpan, e.what());
	}
	return new LocalDeclaration(identifier, expression);
}


Expression* Expression::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	switch (syntacticalAnalyzer->nextToken()->code) {
	case TokenClosingParenthesis:
		syntacticalAnalyzer->back();
		throw parser::EnumerationEnd("expressions", syntacticalAnalyzer->relativeToken()->span);
	case TokenIdentifier:
		syntacticalAnalyzer->back();
		return IdBeginningExpression::parse(syntacticalAnalyzer);
	case TokenThis:
		syntacticalAnalyzer->back();
		return ThisBeginningExpression::parse(syntacticalAnalyzer);
	case TokenBase:
		syntacticalAnalyzer->back();
		return BaseBeginningExpression::parse(syntacticalAnalyzer);
	default:
		syntacticalAnalyzer->back();
		return LiteralBeginningExpression::parse(syntacticalAnalyzer);
	}
}


IdBeginningExpression* IdBeginningExpression::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	Id* identifier;
	try {
		identifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("identifier", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new IdBeginningExpression(identifier);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new IdBeginningExpression(identifier, accessesAndCalls);
}


AccessesAndCalls* AccessesAndCalls::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	switch (syntacticalAnalyzer->nextToken()->code) {
	case TokenDot:
		syntacticalAnalyzer->back();
		return AccessAndFurther::parse(syntacticalAnalyzer);
	case TokenOpeningParenthesis:
		syntacticalAnalyzer->back();
		return CallAndFurther::parse(syntacticalAnalyzer);
	default:
		syntacticalAnalyzer->back();
		throw parser::EnumerationEnd("accesses and calls", syntacticalAnalyzer->relativeToken()->span);
	}
}


AccessAndFurther* AccessAndFurther::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenDot) {
		syntacticalAnalyzer->back();
		throw parser::DotExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	Id* identifier;
	try {
		identifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw parser::RuleParseError("identifier", syntacticalAnalyzer->relativeToken(2)->span, e.what());
	}
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new AccessAndFurther(identifier);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new AccessAndFurther(identifier, accessesAndCalls);
}


CallAndFurther* CallAndFurther::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningParenthesis) {
		syntacticalAnalyzer->back();
		throw parser::OpeningParenthesisExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	Expressions* expressions;
	try {
		expressions = Expressions::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingParenthesis) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingParenthesisExpected("call", errorSpan);
	}
	checkpoint = syntacticalAnalyzer->currentTokenIndex();
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new CallAndFurther(expressions);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new CallAndFurther(expressions, accessesAndCalls);
}


Expressions* Expressions::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	Expression* expression;
	try {
		expression = Expression::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new Expressions();
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("expression", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenComma) {
		syntacticalAnalyzer->back();
		return new Expressions(expression);
	}
	Expressions* expressions;
	try {
		expressions = Expressions::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new Expressions(expression, expressions);
}


ThisBeginningExpression* ThisBeginningExpression::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenThis) {
		syntacticalAnalyzer->back();
		throw parser::ThisExpected("expression", syntacticalAnalyzer->relativeToken()->span);
	}
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new ThisBeginningExpression();
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new ThisBeginningExpression(accessesAndCalls);
}


BaseBeginningExpression* BaseBeginningExpression::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenBase) {
		syntacticalAnalyzer->back();
		throw parser::BaseExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new BaseBeginningExpression();
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new BaseBeginningExpression(accessesAndCalls);
}


LiteralBeginningExpression* LiteralBeginningExpression::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	Literal* literal;
	try {
		literal = Literal::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("literal", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	AccessesAndCalls* accessesAndCalls;
	try {
		accessesAndCalls = AccessesAndCalls::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new LiteralBeginningExpression(literal);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new LiteralBeginningExpression(literal, accessesAndCalls);
}


Literal* Literal::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	switch (syntacticalAnalyzer->nextToken()->code) {
	case TokenInteger:
		syntacticalAnalyzer->back();
		return LiteralInteger::parse(syntacticalAnalyzer);
	case TokenBoolean:
		syntacticalAnalyzer->back();
		return LiteralBoolean::parse(syntacticalAnalyzer);
	case TokenQuote:
		syntacticalAnalyzer->back();
		return LiteralCharacter::parse(syntacticalAnalyzer);
	case TokenFloat:
		syntacticalAnalyzer->back();
		return LiteralFloat::parse(syntacticalAnalyzer);
	case TokenDoubleQuote:
		syntacticalAnalyzer->back();
		return LiteralCharacterArray::parse(syntacticalAnalyzer);
	case TokenOpeningSquareBracket:
		switch (syntacticalAnalyzer->nextToken()->code) {
		case TokenInteger:
			syntacticalAnalyzer->back(2);
			return LiteralIntegerArray::parse(syntacticalAnalyzer);
		case TokenBoolean:
			syntacticalAnalyzer->back(2);
			return LiteralBooleanArray::parse(syntacticalAnalyzer);
		case TokenFloat:
			syntacticalAnalyzer->back(2);
			return LiteralFloatArray::parse(syntacticalAnalyzer);
		case TokenQuote:
			syntacticalAnalyzer->back(2);
			return LiteralCharacterArray::parse(syntacticalAnalyzer);
		}
	case TokenOpeningCurlyBrace:
		switch (syntacticalAnalyzer->nextToken()->code) {
		case TokenInteger:
			syntacticalAnalyzer->back(2);
			return LiteralIntegerList::parse(syntacticalAnalyzer);
		case TokenBoolean:
			syntacticalAnalyzer->back(2);
			return LiteralBooleanList::parse(syntacticalAnalyzer);
		case TokenFloat:
			syntacticalAnalyzer->back(2);
			return LiteralFloatList::parse(syntacticalAnalyzer);
		case TokenQuote:
			syntacticalAnalyzer->back(2);
			return LiteralCharacterList::parse(syntacticalAnalyzer);
		}
	default:
		syntacticalAnalyzer->back();
		throw parser::NotLiteral(syntacticalAnalyzer->relativeToken()->span);
	}
}


LiteralInteger* LiteralInteger::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	if (syntacticalAnalyzer->nextToken()->code == TokenInteger)
		return new LiteralInteger(dynamic_cast<Integer*>(syntacticalAnalyzer->currentToken())->value);
	syntacticalAnalyzer->back();
	throw parser::FailedToParseLiteral("an integer", syntacticalAnalyzer->relativeToken()->span);
}


LiteralBoolean* LiteralBoolean::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	if (syntacticalAnalyzer->nextToken()->code == TokenBoolean)
		return new LiteralBoolean(dynamic_cast<Boolean*>(syntacticalAnalyzer->currentToken())->value);
	syntacticalAnalyzer->back();
	throw parser::FailedToParseLiteral("a boolean", syntacticalAnalyzer->relativeToken()->span);
}


LiteralFloat* LiteralFloat::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	if (syntacticalAnalyzer->nextToken()->code == TokenFloat)
		return new LiteralFloat(dynamic_cast<Float*>(syntacticalAnalyzer->currentToken())->value);
	syntacticalAnalyzer->back();
	throw parser::FailedToParseLiteral("a float", syntacticalAnalyzer->relativeToken()->span);
}


LiteralCharacter* LiteralCharacter::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	if (syntacticalAnalyzer->nextToken()->code == TokenClosingSquareBracket) {
		syntacticalAnalyzer->back();
		throw parser::EnumerationEnd("literal character", syntacticalAnalyzer->relativeToken()->span);
	}
	if (syntacticalAnalyzer->nextToken()->code == TokenCharacter)
		return new LiteralCharacter(dynamic_cast<Character*>(syntacticalAnalyzer->currentToken())->value);
	if (syntacticalAnalyzer->currentToken()->code != TokenQuote) {
		syntacticalAnalyzer->back();
		throw parser::QuoteExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	char character = dynamic_cast<Character*>(syntacticalAnalyzer->nextToken())->value;
	if (syntacticalAnalyzer->nextToken()->code != TokenQuote) {
		syntacticalAnalyzer->back(3);
		throw parser::QuoteExpected(syntacticalAnalyzer->relativeToken(3)->span);
	}
	return new LiteralCharacter(character);
}


LiteralIntegerArray* LiteralIntegerArray::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningSquareBracket) {
		syntacticalAnalyzer->back();
		throw parser::OpeningSquareBracketExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralIntegers* literalIntegers;
	try {
		literalIntegers = LiteralIntegers::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingSquareBracket) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingSquareBracketExpected("literal integer array", errorSpan);
	}
	return new LiteralIntegerArray(literalIntegers);
}


LiteralIntegers* LiteralIntegers::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	LiteralInteger* integer = LiteralInteger::parse(syntacticalAnalyzer);
	if (syntacticalAnalyzer->nextToken()->code != TokenComma) {
		syntacticalAnalyzer->back();
		return new LiteralIntegers(integer);
	}
	LiteralIntegers* integers;
	try {
		integers = LiteralIntegers::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new LiteralIntegers(integer, integers);
}


LiteralBooleanArray* LiteralBooleanArray::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningSquareBracket) {
		syntacticalAnalyzer->back();
		throw parser::OpeningSquareBracketExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralBooleans* literalBooleans;
	try {
		literalBooleans = LiteralBooleans::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingSquareBracket) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingSquareBracketExpected("literal boolean array", errorSpan);
	}
	return new LiteralBooleanArray(literalBooleans);
}


LiteralBooleans* LiteralBooleans::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	LiteralBoolean* boolean = LiteralBoolean::parse(syntacticalAnalyzer);
	if (syntacticalAnalyzer->nextToken()->code != TokenComma) {
		syntacticalAnalyzer->back();
		return new LiteralBooleans(boolean);
	}
	LiteralBooleans* booleans;
	try {
		booleans = LiteralBooleans::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new LiteralBooleans(boolean, booleans);
}


LiteralFloatArray* LiteralFloatArray::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningSquareBracket) {
		syntacticalAnalyzer->back();
		throw parser::OpeningSquareBracketExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralFloats* literalFloats;
	try {
		literalFloats = LiteralFloats::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingSquareBracket) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingSquareBracketExpected("literal float array", errorSpan);
	}
	return new LiteralFloatArray(literalFloats);
}


LiteralFloats* LiteralFloats::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	LiteralFloat* literalFloat = LiteralFloat::parse(syntacticalAnalyzer);
	if (syntacticalAnalyzer->nextToken()->code != TokenComma) {
		syntacticalAnalyzer->back();
		return new LiteralFloats(literalFloat);
	}
	LiteralFloats* literalFloats;
	try {
		literalFloats = LiteralFloats::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new LiteralFloats(literalFloat, literalFloats);
}


LiteralCharacterArray* LiteralCharacterArray::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningSquareBracket 
		&& syntacticalAnalyzer->currentToken()->code != TokenDoubleQuote) {
		syntacticalAnalyzer->back();
		throw parser::OpeningSquareBracketOrQuotesExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralCharacters* literalCharacters;
	try {
		literalCharacters = LiteralCharacters::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingSquareBracket
		&& syntacticalAnalyzer->currentToken()->code != TokenDoubleQuote) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingSquareBracketOrQuotesExpected(errorSpan);
	}
	return new LiteralCharacterArray(literalCharacters);
}


LiteralCharacters* LiteralCharacters::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	LiteralCharacter* literalCharacter = LiteralCharacter::parse(syntacticalAnalyzer);
	if (syntacticalAnalyzer->nextToken()->code != TokenComma)
		syntacticalAnalyzer->back();
	LiteralCharacters* literalCharacters;
	try {
		literalCharacters = LiteralCharacters::parse(syntacticalAnalyzer);
	}
	catch (const parser::EnumerationEnd& e) {
		return new LiteralCharacters(literalCharacter);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	return new LiteralCharacters(literalCharacter, literalCharacters);
}


LiteralIntegerList* LiteralIntegerList::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningCurlyBrace) {
		syntacticalAnalyzer->back();
		throw parser::OpeningCurlyBraceExpected("literal integer list", syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralIntegers* literalIntegers;
	try {
		literalIntegers = LiteralIntegers::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingCurlyBraceExpected("literal integer list", errorSpan);
	}
	return new LiteralIntegerList(literalIntegers);
}


LiteralBooleanList* LiteralBooleanList::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningCurlyBrace) {
		syntacticalAnalyzer->back();
		throw parser::OpeningCurlyBraceExpected("literal boolean list", syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralBooleans* literalBooleans;
	try {
		literalBooleans = LiteralBooleans::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingCurlyBraceExpected("literal boolean list", errorSpan);
	}
	return new LiteralBooleanList(literalBooleans);
}


LiteralFloatList* LiteralFloatList::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningCurlyBrace) {
		syntacticalAnalyzer->back();
		throw parser::OpeningCurlyBraceExpected("literal float list", syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralFloats* literalFloats;
	try {
		literalFloats = LiteralFloats::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingCurlyBraceExpected("literal float list", errorSpan);
	}
	return new LiteralFloatList(literalFloats);
}


LiteralCharacterList* LiteralCharacterList::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningCurlyBrace) {
		syntacticalAnalyzer->back();
		throw parser::OpeningCurlyBraceExpected("literal character list", syntacticalAnalyzer->relativeToken()->span);
	}
	LiteralCharacters* literalCharacters;
	try {
		literalCharacters = LiteralCharacters::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingCurlyBrace) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingCurlyBraceExpected("literal character list", errorSpan);
	}
	return new LiteralCharacterList(literalCharacters);
}


Assignment* Assignment::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	Expression* expression1;
	try {
		expression1 = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("left part expression", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenAssignment) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::AssignmentSignExpected("assignment expression", errorSpan);
	}
	Expression* expression2;
	try {
		expression2 = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("right part expression", errorSpan, e.what());
	}
	return new Assignment(expression1, expression2);
}


IfStatement* IfStatement::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenIf) {
		syntacticalAnalyzer->back();
		throw parser::IfExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	Expression* expression;
	try {
		expression = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw parser::RuleParseError("expression", syntacticalAnalyzer->relativeToken(2)->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenThen) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ThenExpected(errorSpan);
	}
	Statements* statements;
	try {
		statements = Statements::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	syntacticalAnalyzer->skipPossibleNewLines();
	if (syntacticalAnalyzer->nextToken()->code == TokenEnd)
		return new IfStatement(expression, statements);
	if (syntacticalAnalyzer->currentToken()->code != TokenElse) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ElseExpected(errorSpan);
	}
	Statements* statements2;
	try {
		statements2 = Statements::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	syntacticalAnalyzer->skipPossibleNewLines();
	if (syntacticalAnalyzer->nextToken()->code != TokenEnd) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::EndExpected("\"if\" statement", errorSpan);
	}
	return new IfStatement(expression, statements, statements2);
}


WhileStatement* WhileStatement::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenWhile) {
		syntacticalAnalyzer->back();
		throw parser::WhileExpected(syntacticalAnalyzer->relativeToken()->span);
	}
	Expression* expression;
	try {
		expression = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw parser::RuleParseError("expression", syntacticalAnalyzer->relativeToken(2)->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenLoop) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::LoopExpected(errorSpan);
	}
	Statements* statements;
	try {
		statements = Statements::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	syntacticalAnalyzer->skipPossibleNewLines();
	if (syntacticalAnalyzer->nextToken()->code != TokenEnd) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::EndExpected("\"while\" statement", errorSpan);
	}
	return new WhileStatement(expression, statements);
}


ReturnStatement* ReturnStatement::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	if (syntacticalAnalyzer->nextToken()->code != TokenReturn) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->back();
		throw parser::ReturnExpected(errorSpan);
	}
	Expression* expression;
	try {
		expression = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw parser::RuleParseError("expression", syntacticalAnalyzer->relativeToken(2)->span, e.what());
	}
	return new ReturnStatement(expression);
}


ConstructorDeclaration* ConstructorDeclaration::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	ConstructorHead* constructorHead;
	try {
		constructorHead = ConstructorHead::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		throw parser::RuleParseError("constructor head", syntacticalAnalyzer->relativeToken()->span, e.what());
	}
	AnyBody* body;
	try {
		body = AnyBody::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("constructor body", errorSpan, e.what());
	}
	return new ConstructorDeclaration(constructorHead, body);
}


ConstructorHead* ConstructorHead::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenThis) {
		syntacticalAnalyzer->back();
		throw parser::ThisExpected("constructor head", syntacticalAnalyzer->relativeToken()->span);
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenOpeningParenthesis) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::OpeningParenthesisExpected(syntacticalAnalyzer->relativeToken(2)->span);
	}
	ParameterDeclarations* parameterDeclarations;
	try {
		parameterDeclarations = ParameterDeclarations::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->setToken(checkpoint);
		throw e;
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenClosingParenthesis) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::ClosingParenthesisExpected("constructor head", errorSpan);
	}
	return new ConstructorHead(parameterDeclarations);
}


Declaration* Declaration::parse(SyntacticalAnalyzer* syntacticalAnalyzer) {
	int checkpoint = syntacticalAnalyzer->currentTokenIndex();
	if (syntacticalAnalyzer->nextToken()->code != TokenVar) {
		syntacticalAnalyzer->back();
		throw parser::VarExpected("class declaration", syntacticalAnalyzer->relativeToken()->span);
	}
	Id* identifier;
	try {
		identifier = Id::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		syntacticalAnalyzer->back();
		throw parser::RuleParseError("identifier", syntacticalAnalyzer->relativeToken(2)->span, e.what());
	}
	if (syntacticalAnalyzer->nextToken()->code != TokenAssignment) {
		Span errorSpan = syntacticalAnalyzer->currentToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::AssignmentSignExpected("class declaration", errorSpan);
	}
	Expression* expression;
	try {
		expression = Expression::parse(syntacticalAnalyzer);
	}
	catch (const domain_error& e) {
		Span errorSpan = syntacticalAnalyzer->relativeToken()->span;
		syntacticalAnalyzer->setToken(checkpoint);
		throw parser::RuleParseError("expression", errorSpan, e.what());
	}
	return new Declaration(identifier, expression);
}
