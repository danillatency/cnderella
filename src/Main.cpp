#include <iostream>

#include "lexical_analyzer/Tokens.h"
#include "lexical_analyzer/LexicalAnalyzer.h"
#include "syntactical_analyzer/ASTreeGround.h"
#include "semantical_analyzer/SemanticalAnalyzer.h"


using namespace std;


int main(int argc, char* argv[]) {
	OolongLexicalAnalyzer* standardLexer;
	OolongLexicalAnalyzer* lexer;
	try {
		standardLexer = new OolongLexicalAnalyzer("builtin_libraries/Standard.oolong");
		if (argc > 1) {
			string path = argv[1];
			lexer = new OolongLexicalAnalyzer(path);
		}
		else
			lexer = new OolongLexicalAnalyzer("oolong_programs/CelciusToKelvin.oolong");
	}
	catch (const runtime_error& e) {
		cout << e.what() << "\n";
		exit(1);
	}

	try {
		standardLexer->analyze();
		lexer->analyze();
		*standardLexer += *lexer;
	}
	catch (const domain_error& e) {
		cout << e.what() << "\n";
		exit(1);
	}

	standardLexer->printTokens();

	TreeGround ground(standardLexer->getTokenStorage());

	try {
		ground.grow();
	}
	catch (const exception& e) {
		cout << e.what() << "\n";
		exit(1);
	}

	ground.visualize(0, ground.program);

	SemanticalAnalyzer semanticalAnalyzer;

	semanticalAnalyzer.mapToAAST(ground.program);

	semanticalAnalyzer.getTree();

	cout << "\n";
	// cin.ignore();
}
