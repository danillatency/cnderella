#include "TreeVertices.h"


int printPadding = 4;


void static printSpace(unsigned int times) {
	for (int i = 0; i != times; i++) cout << ' ';
}


void Program::selfPrint(unsigned int depth) {
	this->classDeclarations->selfPrint(depth);
	cout << "\n";
}


void ClassDeclarations::selfPrint(unsigned int depth) {
	this->classDeclaration->selfPrint(depth);
	if (this->classDeclarations) {
		cout << "\n";
		this->classDeclarations->selfPrint(depth);
	}
}


void ClassDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[class]\n";
	this->classHead->selfPrint(depth + printPadding);
	this->classBody->selfPrint(depth + printPadding);
}


void ClassHead::selfPrint(unsigned int depth) {
	this->name->selfPrint(depth);
	if (this->baseName) {
		cout << " : ";
		this->baseName->selfPrint(0);
	}
	cout << "\n";
}


void Id::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->identifier;
}


void ClassBody::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[class body]\n";
	this->classStatements->selfPrint(depth + printPadding);
}


void ClassStatements::selfPrint(unsigned int depth) {
	this->classStatement->selfPrint(depth);
	if (this->classStatements)
		this->classStatements->selfPrint(depth);
}


void MethodDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[method]\n";
	this->methodHead->selfPrint(depth + printPadding);
	this->body->selfPrint(depth + printPadding);
}


void MethodHead::selfPrint(unsigned int depth) {
	this->name->selfPrint(depth);
	cout << "\n";
	printSpace(depth);
	cout << "[parameters]\n";
	this->parameterDeclarations->selfPrint(depth + printPadding);
}


void ParameterDeclarations::selfPrint(unsigned int depth) {
	if (this->parameterDeclaration)
		this->parameterDeclaration->selfPrint(depth);
	if (this->parameterDeclarations)
		this->parameterDeclarations->selfPrint(depth);
}


void ParameterDeclaration::selfPrint(unsigned int depth) {
	this->name->selfPrint(depth);
	cout << " : ";
	this->identifier->selfPrint(0);
	cout << "\n";
}


void AnyBody::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[body]\n";
	this->statements->selfPrint(depth + printPadding);
}


void Statements::selfPrint(unsigned int depth) {
	this->statement->selfPrint(depth);
	if (this->statements)
		this->statements->selfPrint(depth);
}


void LocalDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[local declaration]\n";
	this->name->selfPrint(depth + printPadding);
	cout << "\n";
	this->expression->selfPrint(depth + printPadding);
}


void IdBeginningExpression::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[expression]\n";
	this->identifier->selfPrint(depth + printPadding);
	cout << "\n";
	if (this->accessesAndCalls)
		this->accessesAndCalls->selfPrint(depth + printPadding);
}


void AccessAndFurther::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[access] ";
	this->identifier->selfPrint(0);
	if (this->accessesAndCalls) {
		cout << "\n";
		this->accessesAndCalls->selfPrint(depth + printPadding);
	}
}


void CallAndFurther::selfPrint(unsigned int depth) {
	printSpace(depth);
	if (this->expressions->expression)
		cout << "[call with]\n";
	else
		cout << "[call]\n";
	this->expressions->selfPrint(depth + printPadding);
	if (this->accessesAndCalls) {
		printSpace(depth);
		cout << "[arguments end]\n";
		this->accessesAndCalls->selfPrint(depth + printPadding);
	}
}


void Expressions::selfPrint(unsigned int depth) {
	if (this->expression)
		this->expression->selfPrint(depth);
	if (this->expressions) {
		printSpace(depth);
		cout << ", ";
		this->expressions->selfPrint(depth);
	}
}


void ThisBeginningExpression::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[this-expression]\n";
	if (this->accessesAndCalls)
		this->accessesAndCalls->selfPrint(depth + printPadding);
}


void LiteralBeginningExpression::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[expression]\n";
	this->literal->selfPrint(depth + printPadding);
	cout << "\n";
	if (this->accessesAndCalls)
		this->accessesAndCalls->selfPrint(depth + printPadding);
}


void LiteralInteger::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}


void LiteralBoolean::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}


void LiteralFloat::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}


void LiteralCharacter::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << this->value;
}


void LiteralIntegerArray::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '[';
	this->literalIntegers->selfPrint(0);
	cout << ']';
}


void LiteralIntegers::selfPrint(unsigned int depth) {
	this->literalInteger->selfPrint(0);
	if (this->literalIntegers) {
		cout << ", ";
		this->literalIntegers->selfPrint(0);
	}
}


void LiteralBooleanArray::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '[';
	this->literalBooleans->selfPrint(0);
	cout << ']';
}


void LiteralBooleans::selfPrint(unsigned int depth) {
	this->literalBoolean->selfPrint(0);
	if (this->literalBooleans) {
		cout << ", ";
		this->literalBooleans->selfPrint(0);
	}
}


void LiteralFloatArray::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '[';
	this->literalFloats->selfPrint(0);
	cout << ']';
}


void LiteralFloats::selfPrint(unsigned int depth) {
	this->literalFloat->selfPrint(0);
	if (this->literalFloats) {
		cout << ", ";
		this->literalFloats->selfPrint(0);
	}
}


void LiteralCharacterArray::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '"';
	this->literalCharacters->selfPrint(0);
	cout << '"';
}


void LiteralCharacters::selfPrint(unsigned int depth) {
	this->literalCharacter->selfPrint(0);
	if (this->literalCharacters)
		this->literalCharacters->selfPrint(0);
}


void LiteralIntegerList::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '{';
	this->literalIntegers->selfPrint(0);
	cout << '}';
}


void LiteralBooleanList::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '{';
	this->literalBooleans->selfPrint(0);
	cout << '}';
}


void LiteralFloatList::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '{';
	this->literalFloats->selfPrint(0);
	cout << '}';
}


void LiteralCharacterList::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << '{';
	this->literalCharacters->selfPrint(0);
	cout << '}';
}


void Assignment::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[assign]\n";
	this->expression2->selfPrint(depth + printPadding);
	cout << "\n";
	printSpace(depth + printPadding);
	cout << "[to]\n";
	this->expression1->selfPrint(depth + printPadding * 2);
	cout << "\n";
}


void IfStatement::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[if]\n";
	this->condition->selfPrint(depth + printPadding);
	printSpace(depth);
	cout << "[then]\n";
	this->trueStatements->selfPrint(depth + printPadding);
	if (this->falseStatements); {
		printSpace(depth);
		cout << "[else]\n";
		this->falseStatements->selfPrint(depth + printPadding);
	}
}


void WhileStatement::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[while]\n";
	this->condition->selfPrint(depth + printPadding);
	printSpace(depth);
	cout << "[loop]\n";
	this->statements->selfPrint(depth + printPadding);
}


void ReturnStatement::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[return]\n";
	this->expression->selfPrint(depth + printPadding);
}


void ConstructorDeclaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[constructor]\n";
	this->constructorHead->selfPrint(depth + printPadding);
	this->body->selfPrint(depth + printPadding);
}


void ConstructorHead::selfPrint(unsigned int depth) {
	this->parameterDeclarations->selfPrint(depth);
}


void Declaration::selfPrint(unsigned int depth) {
	printSpace(depth);
	cout << "[declaration]\n";
	this->name->selfPrint(depth + printPadding);
	cout << "\n";
	this->expression->selfPrint(depth + printPadding);
}
