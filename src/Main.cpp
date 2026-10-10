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
	for (int i = 0; i < lexer->getTokenStorage().size(); i++) {
		Token* token = lexer->getTokenStorage()[i];
		switch (token->code) {
		case TokenInteger:
			cout << "Integer literal:   " << dynamic_cast<Integer*>(token)->value << "\n";
			break;
		case TokenBoolean:
			cout << "Boolean literal:   " << dynamic_cast<Boolean*>(token)->value << "\n";
			break;
		case TokenCharacter:
			cout << "Character literal: " << dynamic_cast<Character*>(token)->value << "\n";
			break;
		case TokenFloat:
			cout << "Float literal:     " << dynamic_cast<Float*>(token)->value << "\n";
			break;
		case TokenIdentifier:
			cout << "Identifier:        " << dynamic_cast<Identifier*>(token)->value << "\n";
			break;

		default:
			cout << "Reserved word:     " << TokenName[token->code] << "\n";
		}

	}

	cout << "\n";
	// cin.ignore();
}
