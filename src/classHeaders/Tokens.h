#ifndef tokenheaders
#define tokenheaders

#include <string>


using namespace std;


enum TokenCode {
	TokenInteger,
	TokenBoolean,
	TokenCharacter,
	TokenFloat,
	TokenIdentifier,

	TokenClass,
	TokenDot,
	TokenBase,
	TokenMethod,
	TokenWhile,
	TokenLoop,
	TokenReturn,
	TokenArrow,
	TokenIs,
	TokenIf,
	TokenThen,
	TokenElse,
	TokenEnd,
	TokenNewLine,
	TokenVar,
	TokenColon,
	TokenAssignment,
	TokenThis,
	TokenOpeningParenthesis,
	TokenClosingParenthesis,
	TokenComma,
	TokenDoubleQuote,
	TokenOpeningSqureBracket,
	TokenClosingSqureBracket,
	TokenOpeningCurlyBrace,
	TokenClosingCurlyBrace
};


string TokenName[];


class Span {
public:
	unsigned long row;
	unsigned int beginPosition;
	unsigned int endPosition;

	Span(unsigned long row, unsigned int beginPosition, unsigned int endPosition);
};


class Token {
public:
	unsigned int code;
	Span span;

	Token(unsigned int code, Span span);

	virtual ~Token();
};


class Identifier : public Token {
public:
	string value;

	Identifier(Span span, string value);
};


class Integer : public Token {
public:
	int value;

	Integer(Span span, int value);
};


class Float : public Token {
public:
	double value;

	Float(Span span, double value);
};


class Boolean : public Token {
public:
	bool value;

	Boolean(Span span, bool value);
};


class Character : public Token {
public:
	char value;

	Character(Span span, char value);
};


#endif
