#ifndef semanticalanalyzerheader
#define semanticalanalyzerheader

#include "AASTreeVertices.h"
#include "../syntactical_analyzer/ASTreeVertices.h"


using namespace std;


class SemanticalAnalyzer {
private:
	aast::Program* tree;

public:
	aast::Entity* mapToAAST(Entity* vertex);

	void testFindCinderella();

	void visualize();
};


#endif
