#include <stdexcept>

#include "NonTerminalErrors.h"
#include "PrintRowColumn.h"
#include "../lexical_analyzer/Tokens.h"


parser::ClassDeclarationExpected::ClassDeclarationExpected(Span span) : logic_error("Class declaration beginning with \"class\" was expected"s + rowColumn(span)) {}
