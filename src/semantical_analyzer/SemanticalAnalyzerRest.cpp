#include "SemanticalAnalyzer.h"
#include "AASTreeVertices.h"


void SemanticalAnalyzer::visualize() {
	this->tree->selfPrint(0);
}
void SemanticalAnalyzer::testFindCinderella() {
	aast::Entity* cinderella = this->tree->searchHere("Cinderella", nullptr);
	cinderella->selfPrint(0);
}