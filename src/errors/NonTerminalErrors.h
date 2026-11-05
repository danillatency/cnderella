#ifndef nonterminalerrorsheader
#define nonterminalerrorsheader

#include <stdexcept>

#include "../lexical_analyzer/Tokens.h"


using namespace std;


namespace parser {
	class ClassDeclarationExpected : public domain_error {
	public:
		ClassDeclarationExpected(Span span);
	};

	class OutOfTokens : public domain_error {
	public:
		OutOfTokens(Span span);
	};
}


#endif
