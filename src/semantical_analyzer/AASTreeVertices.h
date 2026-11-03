#ifndef aastreeverticesheader
#define aastreeverticesheader

#include <vector>

#include "../lexical_analyzer/Tokens.h"


namespace aast {
	class Entity {
	protected:
		int printPadding = 4;

	public:
		virtual ~Entity() = default;

		//virtual void selfPrint(unsigned int depth) = 0;
	};


	class ClassDeclaration;
	class Program : public Entity {
	private:
		vector<ClassDeclaration*> classDeclarations;

	public:
		void addClassDeclaration(ClassDeclaration* classDeclaration);

		//void selfPrint(unsigned int depth) override;
	};


	class ClassBody;
	class ClassDeclaration : public Entity {
	private:
		string name;
		string baseName;
		ClassBody* classBody;

	public:
		void setClassName(string name);

		void setClassBaseName(string baseName);

		void setClassBody(ClassBody* classBody);

		//void selfPrint(unsigned int depth) override;
	};


	class ClassStatement;
	class ClassBody : public Entity {
	private:
		vector<ClassStatement*> classStatements;

	public:
		void addClassStatement(ClassStatement* classStatement);

		//void selfPrint(unsigned int depth) override;
	};


	class ClassStatement : public Entity {
	public:
		//virtual void selfPrint(unsigned int depth) = 0;
	};


	class ParameterDeclaration;
	class AnyBody;
	class MethodDeclaration : public ClassStatement {
	private:
		string name;
		string type;
		vector<ParameterDeclaration*> parameterDeclarations;
		AnyBody* body;

	public:
		void setName(string name);

		void setType(string type);

		void addParameterDeclaration(ParameterDeclaration* parameterDeclaration);

		void setAnyBody(AnyBody* body);

		//void selfPrint(unsigned int depth) override;
	};


	class ParameterDeclaration : public Entity {
	private:
		string name;
		string type;

	public:
		void setName(string name);

		void setType(string type);

		//void selfPrint(unsigned int depth) override;
	};


	class Statement;
	class AnyBody : public Entity {
	private:
		vector<Statement*> statements;

	public:
		void addStatement(Statement* statement);

		//void selfPrint(unsigned int depth) override;
	};


	class Statement : public Entity {
	public:
		//virtual void selfPrint(unsigned int depth) = 0;
	};


	class Expression;
	class LocalDeclaration : public Statement {
	private:
		string name;
		Expression* expression;

	public:
		void setName(string name);

		void setExpression(Expression* expression);

		//void selfPrint(unsigned int depth) override;
	};


	class AccessOrCall;
	class Expression : public Statement {
	protected:
		vector<AccessOrCall*> accessesAndCalls;

	public:
		void addAccessOrCall(AccessOrCall* accessOrCall);

		//virtual void selfPrint(unsigned int depth) = 0;
	};


	class IdentifierBeginningExpression : public Expression {
	private:
		string identifier;

	public:
		void setIdentifier(string identifier);

		//void selfPrint(unsigned int depth) override;
	};


	class AccessOrCall : public Entity {
	public:
		//virtual void selfPrint(unsigned int depth) = 0;
	};


	class Access : public AccessOrCall {
	private:
		string identifier;

	public:
		void setIdentifier(string identifier);

		//void selfPrint(unsigned int depth) override;
	};


	class Call : public AccessOrCall {
	private:
		vector<Expression*> expressions;

	public:
		void addExpression(Expression* expression);

		//void selfPrint(unsigned int depth) override;
	};


	class ThisBeginningExpression : public Expression {
	public:
		//void selfPrint(unsigned int depth) override;
	};


	class BaseBeginningExpression : public Expression {
	public:
		//void selfPrint(unsigned int depth) override;
	};


	class Assignment;
	class LiteralBeginningExpression : public Expression {
	private:
		vector<Statement*> literalCreationStatements;

	public:
		void addLiteralCreationStatement(Statement* statement);

		//void selfPrint(unsigned int depth) override;
	};


	class Assignment : public Statement {
	private:
		Expression* expression1;
		Expression* expression2;

	public:
		void setExpression1(Expression* expression1);

		void setExpression2(Expression* expression2);

		//void selfPrint(unsigned int depth) override;
	};


	class IfStatement : public Statement {
	private:
		Expression* condition;
		vector<Statement*> trueStatements;
		vector<Statement*> falseStatements;

	public:
		void setCondition(Expression* condition);

		void addTrueStatement(Statement* statement);

		void addFalseStatement(Statement* statement);

		//void selfPrint(unsigned int depth) override;
	};


	class WhileStatement : public Statement {
	private:
		Expression* condition;
		vector<Statement*> statements;

	public:
		void setCondition(Expression* condition);

		void addStatement(Statement* statement);

		//void selfPrint(unsigned int depth) override;
	};


	class ReturnStatement : public Statement {
	private:
		Expression* expression;

	public:
		void setExpression(Expression* expression);

		//void selfPrint(unsigned int depth) override;
	};


	class ConstructorDeclaration : public ClassStatement {
	private:
		vector<ParameterDeclaration*> parameterDeclarations;
		AnyBody* body;

	public:
		void addParameterDeclaration(ParameterDeclaration* parameterDeclaration);

		void setAnyBody(AnyBody* body);

		//void selfPrint(unsigned int depth) override;
	};


	class Declaration : public ClassStatement {
	private:
		string name;
		Expression* expression;

	public:
		void setName(string name);

		void setExpression(Expression* expression);

		//void selfPrint(unsigned int depth) override;
	};
}


#endif
