#include <stdexcept>

#include "SemanticalAnalyzer.h"
#include "AASTreeVertices.h"


void SemanticalAnalyzer::visualize() {
	this->tree->selfPrint(0);
}
void SemanticalAnalyzer::checkDeclarationSignatures() {
	vector<logic_error>* errors = this->tree->checkDeclarationSignatures();
	if (errors->size())
		this->errors->insert(this->errors->end(), errors->begin(), errors->end());
}
void SemanticalAnalyzer::throwErrors() {
	if (!this->errors->size()) return;
	string errors;
	for (int i = 0; i < this->errors->size(); i++)
		errors += this->errors->at(i).what();
	throw logic_error(errors);
}
