#ifndef aastreeverticesheader
#define aastreeverticesheader

#include <vector>
#include <stdexcept>

#include "../lexical_analyzer/Tokens.h"


namespace aast {
	class Expression;
	class EnclosingEntity;
	class Entity {
	protected:
		int printPadding = 4;
		EnclosingEntity* enclosing;
		MultiSpan* multiSpan;

	public:
		virtual ~Entity() = default;

		virtual void selfPrint(unsigned int depth) = 0;

		virtual void setEnclosing(EnclosingEntity* entity);

		void setMultiSpan(MultiSpan multiSpan);
		MultiSpan getMultiSpan();

		EnclosingEntity* getEnclosing();

		virtual vector<logic_error>* checkTypes() = 0;

		// extract to another class
		// bool isInheritedFrom(Expression* entity);
	};


	class ClassDeclaration;
	class ParameterDeclaration;
	class EnclosingEntity : public Entity {
	public:
		virtual Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) = 0;

		virtual Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) = 0;

		ClassDeclaration* getThis();

		virtual vector<logic_error>* checkDeclarationSignatures() = 0;
	};


	class ClassDeclaration;
	class Program : public EnclosingEntity {
	private:
		vector<ClassDeclaration*> classDeclarations;

	public:
		void addClassDeclaration(ClassDeclaration* classDeclaration);

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkDeclarationSignatures() override;

		vector<logic_error>* checkTypes() override;
	};


	class ClassBody;
	class ClassDeclaration : public EnclosingEntity {
	private:
		string name;
		string baseName;
		ClassBody* classBody;

	public:
		void setClassName(string name);
		string getClassName();

		void setClassBaseName(string baseName);
		string getClassBaseName();

		void setClassBody(ClassBody* classBody);
		ClassBody* getClassBody();

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<aast::ParameterDeclaration*>* parameters) override;
		Entity* searchEverywhere(string identifier, vector<aast::ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkTypes() override;

		vector<logic_error>* checkDeclarationSignatures() override;
	};


	class ClassStatement;
	class ClassBody : public EnclosingEntity {
	private:
		vector<ClassStatement*> classStatements;

	public:
		void addClassStatement(ClassStatement* classStatement);
		vector<ClassStatement*>* getClassStatements();

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkDeclarationSignatures() override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class ClassStatement : public EnclosingEntity {
	public:
		virtual void selfPrint(unsigned int depth) = 0;
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
		string getName();

		void setType(string type);
		string getType();

		void addParameterDeclaration(ParameterDeclaration* parameterDeclaration);
		vector<ParameterDeclaration*>* getParameterDeclarations();

		void setAnyBody(AnyBody* body);
		AnyBody* getAnyBody();

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkDeclarationSignatures() override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class ParameterDeclaration : public Entity {
	private:
		string name;
		string type;

	public:
		void setName(string name);
		string getName();

		void setType(string type);
		string getType();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;
	};


	class Statement;
	class ReturnStatement;
	class AnyBody : public EnclosingEntity {
	private:
		vector<Statement*> statements;

	public:
		void addStatement(Statement* statement);
		vector<Statement*>* getStatements();

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkDeclarationSignatures() override;

		vector<logic_error>* checkTypes() override;

		ReturnStatement* getReturnStatement();

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class Statement : public Entity {
	public:
		virtual void selfPrint(unsigned int depth) = 0;
	};


	class Expression;
	class LocalDeclaration : public Statement {
	private:
		string name;
		Expression* expression;

	public:
		void setName(string name);
		string getName();

		void setExpression(Expression* expression);
		Expression* getExpression();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class AccessOrCall;
	class Expression : public Statement {
	protected:
		vector<AccessOrCall*> accessesAndCalls;
		string type = "";

	public:
		void addAccessOrCall(AccessOrCall* accessOrCall);
		vector<AccessOrCall*>* getAccessesAndCalls();

		virtual void selfPrint(unsigned int depth) = 0;

		string getType();

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class IdentifierBeginningExpression : public Expression {
	private:
		string identifier;

	public:
		void setIdentifier(string identifier);
		string getIdentifier();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;
	};


	class AccessOrCall : public Entity {
	public:
		virtual void selfPrint(unsigned int depth) = 0;
	};


	class Access : public AccessOrCall {
	private:
		string identifier;

	public:
		void setIdentifier(string identifier);
		string getIdentifier();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;
	};


	class Call : public AccessOrCall {
	private:
		vector<Expression*> expressions;

	public:
		void addExpression(Expression* expression);
		vector<Expression*> getExpressions();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class ThisBeginningExpression : public Expression {
	public:
		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;
	};


	class BaseBeginningExpression : public Expression {
	public:
		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;
	};


	class LiteralBeginningExpression : public Expression {
	private:
		vector<Statement*> literalCreationStatements;

	public:
		void addLiteralCreationStatement(Statement* statement);

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;
	};


	class Assignment : public Statement {
	private:
		Expression* expression1;
		Expression* expression2;

	public:
		void setExpression1(Expression* expression1);

		void setExpression2(Expression* expression2);

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class IfStatement : public Statement {
	private:
		Expression* condition;
		AnyBody* trueBody = new AnyBody();
		AnyBody* falseBody = new AnyBody();

	public:
		void setCondition(Expression* condition);

		void addTrueStatement(Statement* statement);
		AnyBody* getTrueBody();

		void addFalseStatement(Statement* statement);
		AnyBody* getFalseBody();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class WhileStatement : public Statement {
	private:
		Expression* condition;
		AnyBody* body = new AnyBody();

	public:
		void setCondition(Expression* condition);

		void addStatement(Statement* statement);
		AnyBody* getBody();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class ReturnStatement : public Statement {
	private:
		Expression* expression;

	public:
		void setExpression(Expression* expression);
		Expression* getExpression();

		void selfPrint(unsigned int depth) override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class ConstructorDeclaration : public ClassStatement {
	private:
		vector<ParameterDeclaration*> parameterDeclarations;
		AnyBody* body;

	public:
		void addParameterDeclaration(ParameterDeclaration* parameterDeclaration);
		vector<ParameterDeclaration*>* getParameterDeclarations();

		void setAnyBody(AnyBody* body);
		AnyBody* getAnyBody();

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkDeclarationSignatures() override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};


	class Declaration : public ClassStatement {
	private:
		string name;
		Expression* expression;

	public:
		void setName(string name);
		string getName();

		void setExpression(Expression* expression);
		Expression* getExpression();

		void selfPrint(unsigned int depth) override;

		Entity* searchHere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		Entity* searchEverywhere(string identifier, vector<ParameterDeclaration*>* parameters) override;

		vector<logic_error>* checkDeclarationSignatures() override;

		vector<logic_error>* checkTypes() override;

		void setEnclosing(EnclosingEntity* entity) override;
	};
}


#endif
