#ifndef semanticalanalyzerheader
#define semanticalanalyzerheader


#include <vector>
#include <stdexcept>

#include "AASTreeVertices.h"
#include "../syntactical_analyzer/ASTreeVertices.h"


using namespace std;


class SemanticalAnalyzer {
private:
	aast::Program* tree;
	vector<logic_error>* errors = new vector<logic_error>();

public:
	aast::Entity* mapToAAST(Entity* vertex);

	void checkDeclarationSignatures();

	void throwErrors();

	void visualize();
};


#endif
