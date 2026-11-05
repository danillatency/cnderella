#ifndef nonterminalerrorsheader
#define nonterminalerrorsheader

#include <stdexcept>

#include "../lexical_analyzer/Tokens.h"


using namespace std;


namespace parser {
	class ClassDeclarationExpected : public logic_error {
	public:
		ClassDeclarationExpected(Span span);
	};
}


#endif
