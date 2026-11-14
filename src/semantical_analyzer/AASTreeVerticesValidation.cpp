#include <vector>
#include <stdexcept>

#include "AASTreeVertices.h"
#include "../errors/TerminalErrors.h"


vector<logic_error>* aast::Program::checkDeclarationSignatures() {
	vector<logic_error>* errors = new vector<logic_error>();
	Span test(1, 1, 1);
	for (int i = 1; i < this->classDeclarations.size(); i++)
		for (int j = 0; j < i; j++) {
			if (this->classDeclarations.at(j)->getClassName() == this->classDeclarations.at(i)->getClassName())
				errors->push_back(analyzer::Redeclaration("class", test, test));
		}
	return errors;
}


vector<logic_error>* aast::ClassBody::checkDeclarationSignatures() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::MethodDeclaration::checkDeclarationSignatures() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::AnyBody::checkDeclarationSignatures() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::ConstructorDeclaration::checkDeclarationSignatures() {
	return new vector<logic_error>();
}


vector<logic_error>* aast::Declaration::checkDeclarationSignatures() {
	return new vector<logic_error>();
}