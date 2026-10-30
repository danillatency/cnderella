#include <iostream>

#include "Tokens.h"


using namespace std;


const string TOKEN_NAME[] = {

	"class",
	".",
	"base",
	"method",
	"while",
	"loop",
	"return",
	"=>",
	"is",
	"if",
	"then",
	"else",
	"end",
	"NewLine",
	"var",
	":",
	":=",
	"this",
	"(",
	")",
	",",
	"\"",
	"'",
	"[",
	"]",
	"{",
	"}",

	"LiteralInteger",
	"LiteralBoolean",
	"LiteralCharacter",
	"LiteralFloat",
	"Identifier"
};


Span::Span(unsigned long row, unsigned int beginPosition, unsigned int endPosition) {
	this->row = row;
	this->beginPosition = beginPosition;
	this->endPosition = endPosition;
}


string Token::toString() {
	return "Keyword: " + TOKEN_NAME[this->code];
}
void Token::selfPrint() {
	cout << this->toString() << '\n';
}
Token::Token(TokenCode tokenCode, Span span) : code(tokenCode), span(span) {}


string Identifier::toString() {
	return "Identifier: " + this->value;
}
Identifier::Identifier(Span span, string value) : Token(TokenIdentifier, span), value(value) {}


string Integer::toString() {
	return "LiteralInteger: " + to_string(this->value);
}
Integer::Integer(Span span, int value) : Token(TokenInteger, span), value(value) {}


string Float::toString() {
	return "LiteralFLoat: " + to_string(this->value);
}
Float::Float(Span span, double value) : Token(TokenFloat, span), value(value) {}


string Boolean::toString() {
	return "LiteralBoolean: "s + (this->value ? "true" : "false");
}
Boolean::Boolean(Span span, bool value) : Token(TokenBoolean, span), value(value) {}


string Character::toString() {
	return "Identifier: "s + this->value;
}
Character::Character(Span span, char value) : Token(TokenCharacter, span), value(value) {}
