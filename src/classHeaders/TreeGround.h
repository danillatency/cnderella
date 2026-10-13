#ifndef treegroundheader
#define treegroundheader

#include <vector>

#include "Tokens.h"


using namespace std;


class Program {
public:
	static Program parse(TreeGround* treeGround);
};


class ClassDeclarations {
public:
	static ClassDeclarations parse(TreeGround* treeGround);
};


class ClassDeclaration {
public:
	static ClassDeclaration parse(TreeGround* treeGround);
};


class ClassHead {
public:
	static ClassHead parse(TreeGround* treeGround);
};


class Body {
public:
	static Body parse(TreeGround* treeGround);
};


class Statements {
public:
	static Statements parse(TreeGround* treeGround);
};


class Statement {
public:
	static Statement parse(TreeGround* treeGround);
};


class LocalDeclaration {
public:
	static LocalDeclaration parse(TreeGround* treeGround);
};


class Literal {
public:
	static Literal parse(TreeGround* treeGround);
};


class MethodDeclaration {
public:
	static MethodDeclaration parse(TreeGround* treeGround);
};


class MethodHead {
public:
	static MethodHead parse(TreeGround* treeGround);
};


class ParameterDeclarations {
public:
	static ParameterDeclarations parse(TreeGround* treeGround);
};


class ParameterDeclaration {
public:
	static ParameterDeclaration parse(TreeGround* treeGround);
};


class ConstructorDeclaration {
public:
	static ConstructorDeclaration parse(TreeGround* treeGround);
};


class ConstructorHead {
public:
	static ConstructorHead parse(TreeGround* treeGround);
};


class Expressions {
public:
	static Expressions parse(TreeGround* treeGround);
};


class Expression {
public:
	static Expression parse(TreeGround* treeGround);
};


class Assignment {
public:
	static Assignment parse(TreeGround* treeGround);
};


class IfStatement {
public:
	static IfStatement parse(TreeGround* treeGround);
};


class WhileStatement {
public:
	static WhileStatement parse(TreeGround* treeGround);
};


class ReturnStatement {
public:
	static ReturnStatement parse(TreeGround* treeGround);
};


class TreeGround {
private:
	Program program;
	vector<Token*>& tokens;
	long int currentTokenIndex;

public:
	TreeGround(vector<Token*>& tokens);

	void grow();

	bool outOfTokens();

	Token* currentToken();

	Token* nextToken();

	void back();
};


#endif
