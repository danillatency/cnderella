#include <stdexcept>

#include "TerminalErrors.h"
#include "PrintRowColumn.h"
#include "../lexical_analyzer/Tokens.h"


lexer::CantOpenFile::CantOpenFile(string path) : logic_error("Failed to open "s + '\"' + path + "\"\n") {}


lexer::IncorrectIdentifier::IncorrectIdentifier(Span& span) : logic_error("Incorrect identifier"s + rowColumn(span)) {}


lexer::UnknownToken::UnknownToken(Span& span) : logic_error("Unknown token"s + rowColumn(span)) {}


lexer::UnclosedCharacterLiteral::UnclosedCharacterLiteral(Span& span) : logic_error("Unclosed character literal"s + rowColumn(span)) {}


lexer::ConsecutiveMinusSymbols::ConsecutiveMinusSymbols(Span& span) : logic_error("Consecutive Minus symbols"s + rowColumn(span)) {}