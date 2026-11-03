#ifndef astreegroundheader
#define astreegroundheader

#include <vector>

#include "ASTreeVertices.h"


class TreeGround {
private:
	vector<Token*>& tokens;

public:
	Program* program;
	long int currentTokenIndex;

	TreeGround(vector<Token*>& tokens);

	void grow();

	Token* currentToken();

	string currentTokenStringValue();

	Token* nextToken();

	void back(int steps = 1);

	void visualize();
};


#endif
