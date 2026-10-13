#include <iostream>

#include "classHeaders/Tokens.h"
#include "classHeaders/LexicalAnalyzer.h"
#include "classHeaders/TreeGround.h"


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

	TreeGround ground(lexer->getTokenStorage());
	ground.grow();

	cout << "\n";
	// cin.ignore();
}
