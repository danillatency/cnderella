#include <stdexcept>

#include "NonTerminalErrors.h"
#include "PrintRowColumn.h"
#include "../lexical_analyzer/Tokens.h"


parser::ClassDeclarationExpected::ClassDeclarationExpected(Span span) : domain_error("Class declaration beginning with \"class\" was expected"s + rowColumn(span)) {}


parser::OutOfTokens::OutOfTokens(Span span) : domain_error("Incomplete class declaration due to tokens end"s + rowColumn(span)) {}


parser::IdentifierExpected::IdentifierExpected(Span span) : domain_error("Identifier was expected"s + rowColumn(span)) {}


parser::ArrowExpected::ArrowExpected(string context, Span span) : domain_error("Arrow (=>), pointing to the "s + context + " was expected" + rowColumn(span)) {}


parser::EndExpected::EndExpected(string context, Span span) : domain_error("\"end\" of the "s + context + " was expected" + rowColumn(span)) {}


parser::NotLiteral::NotLiteral(Span span) : domain_error("This was not a literal"s + rowColumn(span)) {}


parser::FailedToParseLiteral::FailedToParseLiteral(string context, Span span) : domain_error("Not "s + context + " literal" + rowColumn(span)) {}


parser::QuoteExpected::QuoteExpected(Span span) : domain_error("A quote for the chracter was expected"s + rowColumn(span)) {}


parser::ColonExpected::ColonExpected(Span span) : domain_error("Parameter declaration colon was expected"s + rowColumn(span)) {}


parser::ClosingParenthesisExpected::ClosingParenthesisExpected(string context, Span span) : domain_error("Closing parenthesis  of the "s + context + " was expected" + rowColumn(span)) {}


parser::ThenExpected::ThenExpected(Span span) : domain_error("\"then\" for the \"if\" statement was expected"s + rowColumn(span)) {}


parser::OpeningCurlyBraceExpected::OpeningCurlyBraceExpected(string context, Span span) : domain_error("Opening curly brace of the "s + context + " was expected" + rowColumn(span)) {}


parser::OpeningParenthesisExpected::OpeningParenthesisExpected(Span span) : domain_error("Opening parenthesis was "s + rowColumn(span)) {}


parser::OpeningSquareBracketExpected::OpeningSquareBracketExpected(Span span) : domain_error("Opening square bracket was expected"s + rowColumn(span)) {}


parser::IfExpected::IfExpected(Span span) : domain_error("\"if\" keyword for the conditional statement was expected"s + rowColumn(span)) {}


parser::OpeningSquareBracketOrQuotesExpected::OpeningSquareBracketOrQuotesExpected(Span span) : domain_error("Opening square bracket or quotes were expected for the characters list"s + rowColumn(span)) {}


parser::WhileExpected::WhileExpected(Span span) : domain_error("\"while\" keyword for the loop statement was expected"s + rowColumn(span)) {}


parser::LoopExpected::LoopExpected(Span span) : domain_error("\"loop\" keyword for the loop statement was expected"s + rowColumn(span)) {}


parser::VarExpected::VarExpected(string context, Span span) : domain_error("\"var\" keyword of the "s + context + " was expected" + rowColumn(span)) {}


parser::AssignmentSignExpected::AssignmentSignExpected(string context, Span span) : domain_error("Assignment (:=) sign of the "s + context + " was expected" + rowColumn(span)) {}


parser::ThisExpected::ThisExpected(string context, Span span) : domain_error("\"this\" keyword for the "s + context + " was expected" + rowColumn(span)) {}


parser::ClosingSquareBracketExpected::ClosingSquareBracketExpected(string context, Span span) : domain_error("Closing square bracket of the "s + context + " was expected" + rowColumn(span)) {}


parser::ClosingSquareBracketOrQuotesExpected::ClosingSquareBracketOrQuotesExpected(Span span) : domain_error("Closing square bracket or quotes are expected for the literal character array"s + rowColumn(span)) {}


parser::ClosingCurlyBraceExpected::ClosingCurlyBraceExpected(string context, Span span) : domain_error("Closing curly brace for the "s + context + " was expected" + rowColumn(span)) {}


parser::ElseExpected::ElseExpected(Span span) : domain_error("\"else\" keyword of the conditional statement was expected"s + rowColumn(span)) {}


parser::ReturnExpected::ReturnExpected(Span span) : domain_error("\"return\" keywword was expected"s + rowColumn(span)) {}


parser::MethodExpected::MethodExpected(Span span) : domain_error("\"method\" keyword was expected"s + rowColumn(span)) {}


parser::BaseExpected::BaseExpected(Span span) : domain_error("\"base\" keyword for the expression was expected"s + rowColumn(span)) {}


parser::CommaExpected::CommaExpected(Span span) : domain_error("Comma between parameter declarations is expected"s + rowColumn(span)) {}


parser::DotExpected::DotExpected(Span span) : domain_error("Access expression dot was expected"s + rowColumn(span)) {}


parser::RuleParseError::RuleParseError(string element, Span span, string reason) : domain_error("Cannot parse the "s + element + rowColumn(span) + "\n\ndue to\n\n" + reason) {}
