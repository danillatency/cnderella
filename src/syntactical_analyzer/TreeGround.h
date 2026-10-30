#ifndef treegroundheader
#define treegroundheader

#include <vector>

#include "TreeVertices.h"


class TreeGround {
private:
	Program* program;
	vector<Token*>& tokens;

public:
	long int currentTokenIndex;

	TreeGround(vector<Token*>& tokens);

	void grow();

	Token* currentToken();

	string currentTokenStringValue();

	Token* nextToken();

	void back(int steps = 1);

	void visualize(unsigned int depth, Entity* entity);
};


#endif
