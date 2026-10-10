#include <iostream>

#include "classHeaders/Tokens.h"
#include "classHeaders/LexicalAnalyzer.h"


using namespace std;


int main() {
	OolongLexicalAnalyzer* lexer;

	try {
		lexer = new OolongLexicalAnalyzer("oolongPrograms/Cindecast.oolong");
	}
	catch (const runtime_error& e) {
		cout << e.what() << "\n";
		exit(1);
	}

	try {
		lexer->analyze();
	}
	catch (const domain_error& e) {
		cout << e.what() << "\n";
		exit(1);
	}

	lexer->printTokens();

	cout << "\n";
	// cin.ignore();
}
