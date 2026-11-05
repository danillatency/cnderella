#include <stdexcept>

#include "NonTerminalErrors.h"
#include "PrintRowColumn.h"
#include "../lexical_analyzer/Tokens.h"


parser::ClassDeclarationExpected::ClassDeclarationExpected(Span span) : domain_error("Class declaration beginning with \"class\" was expected"s + rowColumn(span)) {}


parser::OutOfTokens::OutOfTokens(Span span) : domain_error("Incomplete class declaration due to tokens end"s + rowColumn(span)) {}
