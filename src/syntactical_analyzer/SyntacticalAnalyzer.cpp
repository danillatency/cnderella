#include "SyntacticalAnalyzer.h"
#include "../errors/NonTerminalErrors.h"
#include "../lexical_analyzer/Tokens.h"


SyntacticalAnalyzer::SyntacticalAnalyzer(vector<Token*>& tokens) : tokens(tokens), tokenIndex(-1) {}

void SyntacticalAnalyzer::grow() {
	this->program = Program::parse(this);
}

Token* SyntacticalAnalyzer::currentToken() {
	return this->tokens.at(this->tokenIndex);
}

Token* SyntacticalAnalyzer::token(int index) {
	return this->tokens.at(index);
}
 
int SyntacticalAnalyzer::currentTokenIndex() const {
	return this->tokenIndex;
}

void SyntacticalAnalyzer::setToken(int index) {
	this->tokenIndex = index;
}

string SyntacticalAnalyzer::currentTokenStringValue() {
	return dynamic_cast<Identifier*>(this->currentToken())->value;
}

Token* SyntacticalAnalyzer::nextToken() {
	if (this->tokenIndex + 1 == this->tokens.size())
		throw parser::OutOfTokens(this->currentToken()->span);
	return this->tokens.at(++this->tokenIndex);
}

void SyntacticalAnalyzer::back(int steps) {
	this->tokenIndex -= steps;
}

void SyntacticalAnalyzer::visualize() {
	this->program->selfPrint(0);
}
