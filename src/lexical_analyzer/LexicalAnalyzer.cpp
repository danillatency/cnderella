#include <stdexcept>
#include <iostream>

#include "LexicalAnalyzer.h"


using namespace std;


char& OolongLexicalAnalyzer::further() {
	if (!this->file.get(this->currentCharacter)) {
		this->currentCharacter = NULL;
		return this->currentCharacter;
	}
	this->accumulator += this->currentCharacter;
	this->currentColumn++;
	if (this->currentCharacter == '\n') {
		this->currentColumn = 0;
		this->currentRow++;
	}
	return this->currentCharacter;
}

bool OolongLexicalAnalyzer::correctIntegerCharacter(char& character) {
	return (character > 47) && (character < 58);
}

bool OolongLexicalAnalyzer::correctIdentifierCharacter(char& character) {
	return (character > 64) && (character < 91)
		||
		(character > 96) && (character < 123)
		||
		(character > 47) && (character < 58)
		||
		(character == '-') || (character == '_');
}

bool OolongLexicalAnalyzer::nonSpace() {
	return !isspace(this->currentCharacter) && this->currentCharacter != NULL || this->currentCharacter == '\n';
}

OolongLexicalAnalyzer::OolongLexicalAnalyzer(string programPath) : currentCharacter('a'), currentRow(0), currentColumn(-1), accumulator("") {
	this->programPath = programPath;
	this->file.open(programPath);
	if (!this->file.is_open()) {
		throw runtime_error("Failed to open the file");
	}
}

OolongLexicalAnalyzer::~OolongLexicalAnalyzer() {
	this->file.close();
}

void OolongLexicalAnalyzer::analyze() {
	bool immediatelyFurther = false;
	while (immediatelyFurther || this->further()) {
		immediatelyFurther = false;
		Span span(this->currentRow, this->currentColumn, this->currentColumn + 1);

		switch (this->currentCharacter) {
		case 'c':
			if (this->further() == 'l')
				if (this->further() == 'a')
					if (this->further() == 's')
						if (this->further() == 's')
							if (!this->correctIdentifierCharacter(this->further())) {
								span.endPosition = this->currentColumn;
								this->tokenStorage.push_back(new Token(TokenClass, span));
								this->accumulator = "";
								if (this->nonSpace()) {
									immediatelyFurther = true;
									this->accumulator += this->currentCharacter;
								}
								continue;
							}
			break;

		case '.':
			this->tokenStorage.push_back(new Token(TokenDot, span));
			this->accumulator = "";
			continue;

		case 'b':
			if (this->further() == 'a')
				if (this->further() == 's')
					if (this->further() == 'e')
						if (!this->correctIdentifierCharacter(this->further())) {
							span.endPosition = this->currentColumn;
							this->tokenStorage.push_back(new Token(TokenBase, span));
							this->accumulator = "";
							if (this->nonSpace()) {
								immediatelyFurther = true;
								this->accumulator += this->currentCharacter;
							}
							continue;
						}
			break;

		case 'm':
			if (this->further() == 'e')
				if (this->further() == 't')
					if (this->further() == 'h')
						if (this->further() == 'o')
							if (this->further() == 'd')
								if (!this->correctIdentifierCharacter(this->further())) {
									span.endPosition = this->currentColumn;
									this->tokenStorage.push_back(new Token(TokenMethod, span));
									this->accumulator = "";
									if (this->nonSpace()) {
										immediatelyFurther = true;
										this->accumulator += this->currentCharacter;
									}
									continue;
								}
			break;

		case 'w':
			if (this->further() == 'h')
				if (this->further() == 'i')
					if (this->further() == 'l')
						if (this->further() == 'e')
							if (!this->correctIdentifierCharacter(this->further())) {
								span.endPosition = this->currentColumn;
								this->tokenStorage.push_back(new Token(TokenWhile, span));
								this->accumulator = "";
								if (this->nonSpace()) {
									immediatelyFurther = true;
									this->accumulator += this->currentCharacter;
								}
								continue;
							}
			break;

		case 'l':
			if (this->further() == 'o')
				if (this->further() == 'o')
					if (this->further() == 'p')
						if (!this->correctIdentifierCharacter(this->further())) {
							span.endPosition = this->currentColumn;
							this->tokenStorage.push_back(new Token(TokenLoop, span));
							this->accumulator = "";
							if (this->nonSpace()) {
								immediatelyFurther = true;
								this->accumulator += this->currentCharacter;
							}
							continue;
						}
			break;

		case 'r':
			if (this->further() == 'e')
				if (this->further() == 't')
					if (this->further() == 'u')
						if (this->further() == 'r')
							if (this->further() == 'n')
								if (!this->correctIdentifierCharacter(this->further())) {
									span.endPosition = this->currentColumn;
									this->tokenStorage.push_back(new Token(TokenReturn, span));
									this->accumulator = "";
									if (this->nonSpace()) {
										immediatelyFurther = true;
										this->accumulator += this->currentCharacter;
									}
									continue;
								}
			break;

		case '=':
			if (this->further() != '>')
				throw domain_error("Unknown token on the line: " + to_string(span.row) +
					" from: " + to_string(span.beginPosition) + " to: " + to_string(span.endPosition));
			span.endPosition = this->currentColumn;
			this->tokenStorage.push_back(new Token(TokenArrow, span));
			this->accumulator = "";
			continue;

		case 'i':
			if (this->further() == 's') {
				if (!this->correctIdentifierCharacter(this->further())) {
					span.endPosition = this->currentColumn;
					this->tokenStorage.push_back(new Token(TokenIs, span));
					this->accumulator = "";
					if (this->nonSpace()) {
						immediatelyFurther = true;
						this->accumulator += this->currentCharacter;
					}
					continue;
				}
			} else if (this->currentCharacter == 'f')
				if (!this->correctIdentifierCharacter(this->further())) {
					span.endPosition = this->currentColumn;
					this->tokenStorage.push_back(new Token(TokenIf, span));
					this->accumulator = "";
					if (this->nonSpace()) {
						immediatelyFurther = true;
						this->accumulator += this->currentCharacter;
					}
					continue;
				}
			break;

		case 'e':
			if (this->further() == 'n') {
				if (this->further() == 'd')
					if (!this->correctIdentifierCharacter(this->further())) {
						span.endPosition = this->currentColumn;
						this->tokenStorage.push_back(new Token(TokenEnd, span));
						this->accumulator = "";
						if (this->nonSpace()) {
							immediatelyFurther = true;
							this->accumulator += this->currentCharacter;
						}
						continue;
					}
			} else if (this->currentCharacter == 'l')
				if (this->further() == 's')
					if (this->further() == 'e')
						if (!this->correctIdentifierCharacter(this->further())) {
							span.endPosition = this->currentColumn;
							this->tokenStorage.push_back(new Token(TokenElse, span));
							this->accumulator = "";
							if (this->nonSpace()) {
								immediatelyFurther = true;
								this->accumulator += this->currentCharacter;
							}
							continue;
						}
			break;

		case '\n':
			if (this->tokenStorage.back()->code != TokenNewLine)
				this->tokenStorage.push_back(new Token(TokenNewLine, span));
			this->accumulator = "";
			continue;

		case 'v':
			if (this->further() == 'a')
				if (this->further() == 'r')
					if (!this->correctIdentifierCharacter(this->further())) {
						span.endPosition = this->currentColumn;
						this->tokenStorage.push_back(new Token(TokenVar, span));
						this->accumulator = "";
						if (this->nonSpace()) {
							immediatelyFurther = true;
							this->accumulator += this->currentCharacter;
						}
						continue;
					}
			break;

		case ':':
			if (this->further() == '=') {
				span.endPosition = this->currentColumn;
				this->tokenStorage.push_back(new Token(TokenAssignment, span));
				this->accumulator = "";
			}
			else {
				this->tokenStorage.push_back(new Token(TokenColon, span));
				this->accumulator = "";
				if (this->nonSpace()) {
					immediatelyFurther = true;
					this->accumulator += this->currentCharacter;
				}
			}
			continue;

		case 't':
			if (this->further() == 'h') {
				if (this->further() == 'i') {
					if (this->further() == 's')
						if (!this->correctIdentifierCharacter(this->further())) {
							span.endPosition = this->currentColumn;
							this->tokenStorage.push_back(new Token(TokenThis, span));
							this->accumulator = "";
							if (this->nonSpace()) {
								immediatelyFurther = true;
								this->accumulator += this->currentCharacter;
							}
							continue;
						}
				}
				else if (this->currentCharacter == 'e')
					if (this->further() == 'n')
						if (!this->correctIdentifierCharacter(this->further())) {
							span.endPosition = this->currentColumn;
							this->tokenStorage.push_back(new Token(TokenThen, span));
							this->accumulator = "";
							if (this->nonSpace()) {
								immediatelyFurther = true;
								this->accumulator += this->currentCharacter;
							}
							continue;
						}
			}
			else if (this->currentCharacter == 'r')
				if (this->further() == 'u')
					if (this->further() == 'e')
						if (!this->correctIdentifierCharacter(this->further())) {
							span.endPosition = this->currentColumn;
							this->tokenStorage.push_back(new Boolean(span, true));
							this->accumulator = "";
							if (this->nonSpace()) {
								immediatelyFurther = true;
								this->accumulator += this->currentCharacter;
							}
							continue;
						}
			break;

		case 'f':
			if (this->further() == 'a')
				if (this->further() == 'l')
					if (this->further() == 's')
						if (this->further() == 'e')
							if (!this->correctIdentifierCharacter(this->further())) {
								span.endPosition = this->currentColumn;
								this->tokenStorage.push_back(new Boolean(span, false));
								this->accumulator = "";
								if (this->nonSpace()) {
									immediatelyFurther = true;
									this->accumulator += this->currentCharacter;
								}
								continue;
							}
			break;

		case '(':
			this->tokenStorage.push_back(new Token(TokenOpeningParenthesis, span));
			this->accumulator = "";
			continue;

		case ')':
			this->tokenStorage.push_back(new Token(TokenClosingParenthesis, span));
			this->accumulator = "";
			continue;

		case '{':
			this->tokenStorage.push_back(new Token(TokenOpeningCurlyBrace, span));
			this->accumulator = "";
			continue;

		case '}':
			this->tokenStorage.push_back(new Token(TokenClosingCurlyBrace, span));
			this->accumulator = "";
			continue;

		case '[':
			this->tokenStorage.push_back(new Token(TokenOpeningSquareBracket, span));
			this->accumulator = "";
			continue;

		case ']':
			this->tokenStorage.push_back(new Token(TokenClosingSquareBracket, span));
			this->accumulator = "";
			continue;

		case ',':
			this->tokenStorage.push_back(new Token(TokenComma, span));
			this->accumulator = "";
			continue;

		case '"':
			this->tokenStorage.push_back(new Token(TokenDoubleQuote, span));
			while (this->further() != '"') {
				span.endPosition = this->currentColumn;
				this->tokenStorage.push_back(new Character(span, this->currentCharacter));
			}
			span.endPosition = this->currentColumn;
			this->tokenStorage.push_back(new Token(TokenDoubleQuote, span));
			this->accumulator = "";
			continue;

		case '\'':
			this->tokenStorage.push_back(new Token(TokenQuote, span));
			span.endPosition = this->currentColumn;
			this->tokenStorage.push_back(new Character(span, this->further()));
			if (this->further() != '\'')
				throw domain_error("Unclosed character on the line: " + to_string(span.row) +
					" from: " + to_string(span.beginPosition) + " to: " + to_string(span.endPosition));
			span.endPosition = this->currentColumn;
			this->tokenStorage.push_back(new Token(TokenQuote, span));
			this->accumulator = "";
			continue;

		case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			do {
				this->further();
				if (this->currentCharacter == '-')
					throw domain_error("Mixed minuses: " + to_string(span.row) +
						" from: " + to_string(span.beginPosition) + " to: " + to_string(span.endPosition));
			} while (this->correctIntegerCharacter(this->currentCharacter));
			if (this->currentCharacter == '.')
				if (!this->correctIntegerCharacter(this->further())) {
					span.endPosition = this->currentColumn - 2;
					this->accumulator.pop_back();
					this->accumulator.pop_back();
					this->tokenStorage.push_back(new Integer(span, stoi(this->accumulator)));
					Span extraSpan(this->currentRow, this->currentColumn - 1, this->currentColumn - 1);
					this->tokenStorage.push_back(new Token(TokenDot, extraSpan));
				}
				else {
					while (this->correctIntegerCharacter(this->further())) continue;
					span.endPosition = this->currentColumn - 1;
					this->accumulator.pop_back();
					this->tokenStorage.push_back(new Float(span, stod(this->accumulator)));
				}
			else {
				span.endPosition = this->currentColumn;
				this->accumulator.pop_back();
				this->tokenStorage.push_back(new Integer(span, stoi(this->accumulator)));
			}
			this->accumulator = "";
			if (this->nonSpace()) {
				immediatelyFurther = true;
				this->accumulator += this->currentCharacter;
			}
			continue;
		}

		if (isspace(this->currentCharacter)) {
			this->accumulator.pop_back();
			if (this->accumulator != "") {
				span.endPosition = this->currentColumn;
				this->tokenStorage.push_back(new Identifier(span, this->accumulator));
				this->accumulator = "";
			}
			continue;
		}
		while (this->correctIdentifierCharacter(this->currentCharacter))
			this->further();
		span.endPosition = this->currentColumn;
		this->accumulator.pop_back();
		this->tokenStorage.push_back(new Identifier(span, this->accumulator));
		this->accumulator = "";
		if (this->nonSpace()) {
			immediatelyFurther = true;
			this->accumulator += this->currentCharacter;
		}
	}
}

void OolongLexicalAnalyzer::printTokens() {
	for (int i = 0; i < this->tokenStorage.size(); i++) {
		Token* token = this->tokenStorage[i];
		switch (token->code) {
		case TokenInteger:
			cout << i << ". Integer literal:   " << dynamic_cast<Integer*>(token)->value << "\n";
			break;
		case TokenBoolean:
			cout << i << ". Boolean literal:   " << dynamic_cast<Boolean*>(token)->value << "\n";
			break;
		case TokenCharacter:
			cout << i << ". Character literal: " << dynamic_cast<Character*>(token)->value << "\n";
			break;
		case TokenFloat:
			cout << i << ". Float literal:     " << dynamic_cast<Float*>(token)->value << "\n";
			break;
		case TokenIdentifier:
			cout << i << ". Identifier:        " << dynamic_cast<Identifier*>(token)->value << "\n";
			break;

		default:
			cout << i << ". Reserved word:     " << TokenName[token->code] << "\n";
		}

	}
}

vector<Token*>& OolongLexicalAnalyzer::getTokenStorage() {
	return this->tokenStorage;
}
