#include <stdexcept>

#include "NonTerminalErrors.h"
#include "PrintRowColumn.h"
#include "../lexical_analyzer/Tokens.h"


parser::ClassDeclarationExpected::ClassDeclarationExpected(Span span) : domain_error("Class declaration beginning with \"class\" was expected"s + rowColumn(span)) {}


parser::OutOfTokens::OutOfTokens(Span span) : domain_error("Incomplete class declaration due to tokens end"s + rowColumn(span)) {}


parser::IdentifierExpected::IdentifierExpected(Span span) : domain_error("Identifier was expected"s + rowColumn(span)) {}


parser::ArrowExpected::ArrowExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::EndExpected::EndExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::FailedToParse::FailedToParse(Span span) : domain_error(""s + rowColumn(span)) {}


parser::QuoteExpected::QuoteExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ColonExpected::ColonExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ClosingParenthesisExpected::ClosingParenthesisExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ThenExpected::ThenExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::OpeningCurlyBraceExpected::OpeningCurlyBraceExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::AssignmentExpected::AssignmentExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::OpeningParenthesisExpected::OpeningParenthesisExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::OpeningSquareBracketExpected::OpeningSquareBracketExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::IfExpected::IfExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::OpeningSquareBracketOrQuotesExpected::OpeningSquareBracketOrQuotesExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::UnexpectedToken::UnexpectedToken(Span span) : domain_error(""s + rowColumn(span)) {}


parser::WhileExpected::WhileExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::LoopExpected::LoopExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::VarExpected::VarExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::AssignmentSignExpected::AssignmentSignExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ThisExpected::ThisExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ClosingSquareBracketExpected::ClosingSquareBracketExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ClosingSquareBracketOrQuotesExpected::ClosingSquareBracketOrQuotesExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ClosingCurlyBraceExpected::ClosingCurlyBraceExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ElseExpected::ElseExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::ReturnExpected::ReturnExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::MethodExpected::MethodExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::BaseExpected::BaseExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::CommaExpected::CommaExpected(Span span) : domain_error(""s + rowColumn(span)) {}


parser::DotExpected::DotExpected(Span span) : domain_error(""s + rowColumn(span)) {}


