#include "TreeGround.h"


TreeGround::TreeGround(vector<Token*>& tokens) : tokens(tokens), currentTokenIndex(-1) {}

void TreeGround::grow() {
	this->program = Program::parse(this);
	this->visualize(0, this->program);
}

Token* TreeGround::currentToken() {
	return this->tokens.at(this->currentTokenIndex);
}

string TreeGround::currentTokenStringValue() {
	return dynamic_cast<Identifier*>(this->currentToken())->value;
}

Token* TreeGround::nextToken() {
	return this->tokens.at(++this->currentTokenIndex);
}

void TreeGround::back(int steps) {
	this->currentTokenIndex -= steps;
}

void TreeGround::visualize(unsigned int depth, Entity* entity) {
	entity->selfPrint(depth);
}
