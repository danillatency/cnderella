#include "Tokens.h"


using namespace std;


string TokenName[] = {
	"LiteralInteger",
	"LiteralBoolean",
	"LiteralCharacter",
	"LiteralFloat",
	"LiteralIdentifier",

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
	"}"
};


Span::Span(unsigned long row, unsigned int beginPosition, unsigned int endPosition) {
	this->row = row;
	this->beginPosition = beginPosition;
	this->endPosition = endPosition;
}


Token::Token(unsigned int code, Span span) : code(code), span(span) {}
Token::~Token() = default;


Identifier::Identifier(Span span, string value) : Token(TokenIdentifier, span) {
	this->value = value;
}


Integer::Integer(Span span, int value) : Token(TokenInteger, span) {
	this->value = value;
}


Float::Float(Span span, double value) : Token(TokenFloat, span) {
	this->value = value;
}


Boolean::Boolean(Span span, bool value) : Token(TokenBoolean, span) {
	this->value = value;
}


Character::Character(Span span, char value) : Token(TokenCharacter, span) {
	this->value = value;
}
