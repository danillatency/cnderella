#ifndef astreegroundheader
#define astreegroundheader

#include <vector>

#include "ASTreeVertices.h"


class SyntacticalAnalyzer {
private:
	vector<Token*>& tokens;
	Program* program;
	long int tokenIndex;

public:
	SyntacticalAnalyzer(vector<Token*>& tokens);

	void grow();

	Token* currentToken();

	int currentTokenIndex() const;

	void setToken(int index);

	string currentTokenStringValue();

	Token* nextToken();

	void back(int steps = 1);

	void visualize();
};


#endif
