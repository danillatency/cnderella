#include "SyntacticalAnalyzer.h"
#include "../errors/NonTerminalErrors.h"
#include "../lexical_analyzer/Tokens.h"


SyntacticalAnalyzer::SyntacticalAnalyzer(vector<Token*>& tokens) : tokens(tokens), tokenIndex(-1) {}

void SyntacticalAnalyzer::growTree() {
	this->program = Program::parse(this);
}

Program* SyntacticalAnalyzer::getProgram() {
	return this->program;
}

Token* SyntacticalAnalyzer::currentToken() {
	return this->tokens.at(this->tokenIndex);
}

Token* SyntacticalAnalyzer::token(int index) {
	return this->tokens.at(index);
}

Token* SyntacticalAnalyzer::relativeToken(int shift) {
	if (this->tokenIndex + shift >= this->tokens.size())
		return this->tokens.at(this->tokens.size() - 1);
	return this->tokens.at(this->tokenIndex + shift);
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

void SyntacticalAnalyzer::skipPossibleNewLines() {
	if (this->nextToken()->code != TokenNewLine) this->back();
}

void SyntacticalAnalyzer::visualize() {
	this->program->selfPrint(0);
}
