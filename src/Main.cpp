#include <iostream>
#include <stdexcept>

#include "lexical_analyzer/Tokens.h"
#include "lexical_analyzer/LexicalAnalyzer.h"
#include "syntactical_analyzer/SyntacticalAnalyzer.h"
#include "semantical_analyzer/SemanticalAnalyzer.h"


using namespace std;


static string usePlaceholder(int argc, char** argv) {
	if (argc > 1) return argv[1];
	return "oolong_programs/CelciusToKelvin.oolong";
}


int main(int argc, char* argv[]) {
	try {
		//OolongLexicalAnalyzer standardLexer("builtin_libraries/Standard.oolong");
		OolongLexicalAnalyzer standardLexer(usePlaceholder(argc, argv));
		//OolongLexicalAnalyzer lexer(usePlaceholder(argc, argv));
		standardLexer.analyze();
		//lexer.analyze();
		//standardLexer += lexer;

		standardLexer.printTokens();

		SyntacticalAnalyzer ground(standardLexer.getTokenStorage());
		ground.growTree();

		ground.visualize();

		SemanticalAnalyzer semanticalAnalyzer;

		semanticalAnalyzer.mapToAAST(ground.getProgram());

		semanticalAnalyzer.visualize();

		cout << "\n";
		// cin.ignore();
	}
	catch (const exception& e) {
		cout << e.what() << endl;
		exit(1);
	}
}
