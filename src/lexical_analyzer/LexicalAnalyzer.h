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
	int previousColumn;
	char currentCharacter;
	string accumulator;

	char& further();

	bool correctIntegerCharacter(char& character);

	bool correctIdentifierCharacter(char& character);

	bool currentlyNonSpace();

public:
	OolongLexicalAnalyzer(string programPath);

	~OolongLexicalAnalyzer();

	OolongLexicalAnalyzer& operator+=(const OolongLexicalAnalyzer& other);

	vector<Token*>& getTokenStorage();

	void printTokens();

	void analyze();
};


#endif
