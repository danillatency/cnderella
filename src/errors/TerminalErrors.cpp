#include <stdexcept>

#include "TerminalErrors.h"
#include "PrintRowColumn.h"
#include "../lexical_analyzer/Tokens.h"


lexer::CantOpenFile::CantOpenFile(string path) : logic_error("Failed to open "s + '\"' + path + "\"\n") {}


lexer::IncorrectIdentifier::IncorrectIdentifier(Span& span) : logic_error("Incorrect identifier"s + rowColumn(span)) {}


lexer::UnknownToken::UnknownToken(Span& span) : logic_error("Unknown token"s + rowColumn(span)) {}


lexer::UnclosedCharacterLiteral::UnclosedCharacterLiteral(Span& span) : logic_error("Unclosed character literal"s + rowColumn(span)) {}


lexer::ConsecutiveMinusSymbols::ConsecutiveMinusSymbols(Span& span) : logic_error("Consecutive Minus symbols"s + rowColumn(span)) {}


analyzer::Redeclaration::Redeclaration(string object, Span& previousSpan, Span& span) : logic_error("The "s + object + " on" + rowColumn(previousSpan) + "\nwas redeclared on" + rowColumn(span) + "\n\n") {}


analyzer::Undeclared::Undeclared(string object, Span& span) : logic_error("This "s + object + " is undeclared" + rowColumn(span) + "\n\n") {}


analyzer::IllegalValue::IllegalValue(string value, Span& span) : logic_error("Values of type"s + value + " are not allowed" + rowColumn(span) + "\n\n") {}


analyzer::TypesMismatch::TypesMismatch(string expected, string met, Span& span) : logic_error(expected + " was expected, but " + met + " was met" + rowColumn(span) + "\n\n") {}