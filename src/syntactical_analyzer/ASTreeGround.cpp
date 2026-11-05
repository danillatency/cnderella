#include "ASTreeGround.h"
#include "../errors/NonTerminalErrors.h"


TreeGround::TreeGround(vector<Token*>& tokens) : tokens(tokens), currentTokenIndex(-1) {}

void TreeGround::grow() {
	this->program = Program::parse(this);
}

Token* TreeGround::currentToken() {
	return this->tokens.at(this->currentTokenIndex);
}

string TreeGround::currentTokenStringValue() {
	return dynamic_cast<Identifier*>(this->currentToken())->value;
}

Token* TreeGround::nextToken() {
	if (this->currentTokenIndex + 1 == this->tokens.size())
		throw parser::OutOfTokens();
	return this->tokens.at(++this->currentTokenIndex);
}

void TreeGround::back(int steps) {
	this->currentTokenIndex -= steps;
}

void TreeGround::visualize() {
	this->program->selfPrint(0);
}
