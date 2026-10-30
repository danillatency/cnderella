#ifndef tokenheaders
#define tokenheaders

#include <string>


using namespace std;


enum TokenCode {
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
	TokenQuote,
	TokenOpeningSquareBracket,
	TokenClosingSquareBracket,
	TokenOpeningCurlyBrace,
	TokenClosingCurlyBrace,

	TokenInteger,
	TokenBoolean,
	TokenCharacter,
	TokenFloat,
	TokenIdentifier
};


class Span {
public:
	unsigned long row;
	unsigned int beginPosition;
	unsigned int endPosition;

	Span(unsigned long row, unsigned int beginPosition, unsigned int endPosition);
};


class Token {
public:
	TokenCode code;
	Span span;

	Token(TokenCode tokenCode, Span span);

	virtual string toString();

	void selfPrint();

	virtual ~Token() = default;
};


class Identifier : public Token {
public:
	string value;

	Identifier(Span span, string value);

	string toString() override;
};


class Integer : public Token {
public:
	int value;

	Integer(Span span, int value);

	string toString() override;
};


class Float : public Token {
public:
	double value;

	Float(Span span, double value);

	string toString() override;
};


class Boolean : public Token {
public:
	bool value;

	Boolean(Span span, bool value);

	string toString() override;
};


class Character : public Token {
public:
	char value;

	Character(Span span, char value);

	string toString() override;
};


#endif
