#ifndef terminalerrorsheader
#define terminalerrorsheader

#include <stdexcept>

#include "../lexical_analyzer/Tokens.h"


using namespace std;


namespace lexer {
	class CantOpenFile : public logic_error {
	public:
		CantOpenFile(string path);
	};


	class IncorrectIdentifier : public logic_error {
	public:
		IncorrectIdentifier(Span& span);
	};


	class UnknownToken : public logic_error {
	public:
		UnknownToken(Span& span);
	};


	class UnclosedCharacterLiteral : public logic_error {
	public:
		UnclosedCharacterLiteral(Span& span);
	};


	class ConsecutiveMinusSymbols : public logic_error {
	public:
		ConsecutiveMinusSymbols(Span& span);
	};
}


#endif
