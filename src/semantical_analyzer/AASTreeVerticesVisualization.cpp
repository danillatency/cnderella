#include <iostream>
#include <vector>
#include <cmath>

#include "AASTreeVertices.h"


using namespace std;


static vector<int> sticks;
static void printSpace(unsigned int times) {
	for (int i = 0; i != times; i++) { 
		if (!sticks.size()) { 
			cout << ' ';
			continue; 
		}
		int index = sticks.size() / 2;
		for (int j = 0; j < log2(sticks.size()); j++) {
			if (sticks.at(index) == i) {
				cout << '|';
				index = -1;
				break;
			}
			else if (sticks.at(index) > i)
				index /= 2;
			else
				index = (index + sticks.size()) / 2;
		}
		if (index == -1)
			continue;
		if (sticks.at(index) == i)
			cout << '|';
		else
			cout << ' ';
	}
}


void aast::Program::selfPrint(unsigned int depth) {
	for (int i = 0; i < this->classDeclarations.size(); i++)
		this->classDeclarations.at(i)->selfPrint(0);

}


void aast::ClassDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[class]" << endl;
	sticks.push_back(depth);
	printSpace(depth);
	cout << "|-" << this->name << endl;
	if (this->baseName != "") {
		printSpace(depth);
		cout << "|-[inherits] " << this->baseName << endl;
	}
	sticks.pop_back();
	printSpace(depth);
	cout << "\\_";
	this->classBody->selfPrint(depth);
}


void aast::ClassBody::selfPrint(unsigned int depth) {
	cout << "[class-body]\n";
	sticks.push_back(depth + this->printPadding);
	for (int i = 0; i < this->classStatements.size() - 1; i++) {
		printSpace(depth + this->printPadding + 1);
		cout << '-';
		this->classStatements.at(i)->selfPrint(depth + this->printPadding);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->classStatements.at(this->classStatements.size() - 1)->selfPrint(depth + this->printPadding);
}


void aast::MethodDeclaration::selfPrint(unsigned int depth) {
	cout << "[method]\n";
	sticks.push_back(depth + this->printPadding);
	printSpace(depth + this->printPadding + 1);
	cout << "-" << this->name << endl;
	if (this->type != "") {
		printSpace(depth + this->printPadding + 1);
		cout << "-[returns] " << this->type << endl;
	}
	if (this->parameterDeclarations.size()) {
		printSpace(depth + this->printPadding + 1);
		cout << "-[parameters]\n";
		sticks.push_back(depth + this->printPadding * 2);
		for (int i = 1; i < this->parameterDeclarations.size(); i++) {
			printSpace(depth + this->printPadding * 2 + 1);
			cout << '-';
			this->parameterDeclarations.at(i - 1)->selfPrint(depth + this->printPadding * 2);
		}
		sticks.pop_back();
		printSpace(depth + this->printPadding * 2);
		cout << "\\_";
		this->parameterDeclarations.at(this->parameterDeclarations.size() - 1)->selfPrint(depth + this->printPadding * 2);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->body->selfPrint(depth + this->printPadding);
}


void aast::ParameterDeclaration::selfPrint(unsigned int depth) {
	cout << this->name << " [of type] " << this->type << endl;
}


void aast::AnyBody::selfPrint(unsigned int depth) {
	cout << "[body]\n";
	sticks.push_back(depth + this->printPadding);
	for (int i = 0; i < this->statements.size() - 1; i++) {
		printSpace(depth + this->printPadding + 1);
		cout << '-';
		this->statements.at(i)->selfPrint(depth + this->printPadding);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->statements.at(this->statements.size() - 1)->selfPrint(depth + this->printPadding);
}


void aast::LocalDeclaration::selfPrint(unsigned int depth) {
	cout << "[local-declaration]\n";
	printSpace(depth + this->printPadding);
	cout << "|-" << (this->name.empty() ? "[anonymous]" : this->name) << endl;
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->expression->selfPrint(depth + this->printPadding);
}


void aast::IdentifierBeginningExpression::selfPrint(unsigned int depth) {
	cout << "[identifier-expression]\n";
	printSpace(depth + this->printPadding);
	cout << "\\_" << this->identifier << endl;
	for (int i = 0; i < this->accessesAndCalls.size(); i++) {
		printSpace(depth + this->printPadding * (i + 2));
		cout << "\\_";
		if (i < this->accessesAndCalls.size() - 1)
			sticks.push_back(depth + this->printPadding * (i + 3));
		this->accessesAndCalls.at(i)->selfPrint(depth + this->printPadding * (i + 2));
		if (i < this->accessesAndCalls.size() - 1)
			sticks.pop_back();
	}
}


void aast::Access::selfPrint(unsigned int depth) {
	cout << "[access]" << this->identifier << endl;
}


void aast::Call::selfPrint(unsigned int depth) {
	cout << "[call]\n";
	if (this->expressions.size()) {
		printSpace(depth + this->printPadding * 2);
		cout << "\\_[arguments]\n";
		sticks.push_back(depth + this->printPadding * 3);
		for (int i = 1; i < this->expressions.size(); i++) {
			printSpace(depth + this->printPadding * 3 + 1);
			cout << '-';
			this->expressions.at(i - 1)->selfPrint(depth + this->printPadding * 3);
		}
		sticks.pop_back();
		printSpace(depth + this->printPadding * 3);
		cout << "\\_";
		this->expressions.at(this->expressions.size() - 1)->selfPrint(depth + this->printPadding * 3);
	}
}


void aast::ThisBeginningExpression::selfPrint(unsigned int  depth) {
	cout << "[this-expression]\n";
	for (int i = 0; i < this->accessesAndCalls.size(); i++) {
		printSpace(depth + this->printPadding * (i + 1));
		cout << "\\_";
		this->accessesAndCalls.at(i)->selfPrint(depth + this->printPadding * (i + 1));
	}
}


void aast::BaseBeginningExpression::selfPrint(unsigned int  depth) {
	cout << "[base-expression]\n";
	for (int i = 0; i < this->accessesAndCalls.size(); i++) {
		printSpace(depth + this->printPadding * (i + 1));
		cout << "\\_";
		this->accessesAndCalls.at(i)->selfPrint(depth + this->printPadding * (i + 1));
	}
}


void aast::LiteralBeginningExpression::selfPrint(unsigned int depth) {
	cout << "[literal-expression]\n";
	if (this->accessesAndCalls.size())
		sticks.push_back(depth + this->printPadding);
	printSpace(depth + this->printPadding * 2);
	cout << "[creation-statements]\n";
	sticks.push_back(depth + this->printPadding * 2);
	for (int i = 0; i < this->literalCreationStatements.size() - 1; i++) {
		printSpace(depth + this->printPadding * 2 + 1);
		cout << '-';
		this->literalCreationStatements.at(i)->selfPrint(depth + this->printPadding * 2);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding * 2);
	cout << "\\_";
	this->literalCreationStatements.at(this->literalCreationStatements.size() - 1)->selfPrint(depth + this->printPadding * 2);
	if (this->accessesAndCalls.size()) {
		sticks.pop_back();
		for (int i = 0; i < this->accessesAndCalls.size(); i++) {
			printSpace(depth + this->printPadding * (i + 1));
			cout << "\\_";
			this->accessesAndCalls.at(i)->selfPrint(depth + this->printPadding * (i + 2));
		}
	}
}


void aast::Assignment::selfPrint(unsigned int depth) {
	cout << "[assignment]\n";
	sticks.push_back(depth + this->printPadding);
	printSpace(depth + this->printPadding * 2);
	cout << "\\_";
	this->expression2->selfPrint(depth + this->printPadding * 2);
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->expression1->selfPrint(depth + this->printPadding);
}


void aast::IfStatement::selfPrint(unsigned int depth) {
	cout << "[if]\n";
	sticks.push_back(depth + this->printPadding);
	printSpace(depth + this->printPadding + 1);
	cout << '-';
	this->condition->selfPrint(depth + this->printPadding);
	printSpace(depth + this->printPadding + 1);
	cout << "-[then]\n";
	sticks.push_back(depth + this->printPadding * 2);
	for (int i = 1; i < this->trueBody.getStatements()->size(); i++) {
		printSpace(depth + this->printPadding * 2 + 1);
		cout << '-';
		this->trueBody.getStatements()->at(i - 1)->selfPrint(depth + this->printPadding * 2);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding * 2);
	cout << "\\_";
	this->trueBody.getStatements()->at(this->trueBody.getStatements()->size() - 1)->selfPrint(depth + this->printPadding * 2);
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_[else]\n";
	sticks.push_back(depth + this->printPadding * 2);
	for (int i = 1; i < this->falseBody.getStatements()->size(); i++) {
		printSpace(depth + this->printPadding * 2 + 1);
		cout << '-';
		this->falseBody.getStatements()->at(i - 1)->selfPrint(depth + this->printPadding * 2);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding * 2);
	cout << "\\_";
	this->falseBody.getStatements()->at(this->falseBody.getStatements()->size() - 1)->selfPrint(depth + this->printPadding * 2);
}


void aast::WhileStatement::selfPrint(unsigned int depth) {
	cout << "[while]\n";
	sticks.push_back(depth + this->printPadding);
	printSpace(depth + this->printPadding + 1);
	cout << '-';
	this->condition->selfPrint(depth + this->printPadding);
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_[loop]\n";
	sticks.push_back(depth + this->printPadding * 2);
	for (int i = 1; i < this->body.getStatements()->size(); i++) {
		printSpace(depth + this->printPadding * 2 + 1);
		cout << '-';
		this->body.getStatements()->at(i - 1)->selfPrint(depth + this->printPadding * 2);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding * 2);
	cout << "\\_";
	this->body.getStatements()->at(this->body.getStatements()->size() - 1)->selfPrint(depth + this->printPadding * 2);
}


void aast::ReturnStatement::selfPrint(unsigned int depth) {
	cout << "[return]\n";
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->expression->selfPrint(depth + this->printPadding);
}


void aast::ConstructorDeclaration::selfPrint(unsigned int depth) {
	cout << "[constructor]\n";
	sticks.push_back(depth + this->printPadding);
	if (this->parameterDeclarations.size()) {
		printSpace(depth + this->printPadding + 1);
		cout << "-[parameters]\n";
		sticks.push_back(depth + this->printPadding * 2);
		for (int i = 1; i < this->parameterDeclarations.size(); i++) {
			printSpace(depth + this->printPadding * 2 + 1);
			cout << '-';
			this->parameterDeclarations.at(i - 1)->selfPrint(depth + this->printPadding * 2);
		}
		sticks.pop_back();
		printSpace(depth + this->printPadding * 2);
		cout << "\\_";
		this->parameterDeclarations.at(this->parameterDeclarations.size() - 1)->selfPrint(depth + this->printPadding * 2);
	}
	sticks.pop_back();
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->body->selfPrint(depth + this->printPadding);
}


void aast::Declaration::selfPrint(unsigned int depth) {
	cout << "[declaration]\n";
	printSpace(depth + this->printPadding);
	cout << "|- " << this->name << endl;
	printSpace(depth + this->printPadding);
	cout << "\\_";
	this->expression->selfPrint(depth + this->printPadding);
}
