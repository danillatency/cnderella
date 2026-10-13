#ifndef lexicalanalyzerheader
#define lexicalanalyzerheader

#include <vector>
#include <string>
#include <fstream>

#include "Tokens.h"


using namespace std;


class OolongLexicalAnalyzer {
private:
	string programPath;
	ifstream file;
	vector<Token*> tokenStorage;
	unsigned long currentRow;
	int currentColumn;
	char currentCharacter;
	string accumulator;

	char& further();

	bool correctIntegerCharacter(char& character);

	bool correctIdentifierCharacter(char& character);

	bool nonSpace();

public:
	OolongLexicalAnalyzer(string programPath);

	~OolongLexicalAnalyzer();

	void analyze();

	void printTokens();

	vector<Token*>& getTokenStorage();
};

#endif
