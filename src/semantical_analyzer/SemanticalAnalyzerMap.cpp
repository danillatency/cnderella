#include <iostream>
#include <cmath>

#include "SemanticalAnalyzer.h"
#include "../syntactical_analyzer/ASTreeVertices.h"


using namespace std;


void populateLiteralCreationStatements(aast::LiteralBeginningExpression& expression, Literal* oldLiteral) {
    LiteralInteger* oldLiteralInteger = dynamic_cast<LiteralInteger*>(oldLiteral);
    LiteralBoolean* oldLiteralBoolean = dynamic_cast<LiteralBoolean*>(oldLiteral);
    LiteralFloat* oldLiteralFloat = dynamic_cast<LiteralFloat*>(oldLiteral);
    LiteralCharacter* oldLiteralCharacter = dynamic_cast<LiteralCharacter*>(oldLiteral);
    LiteralIntegerArray* oldLiteralIntegerArray = dynamic_cast<LiteralIntegerArray*>(oldLiteral);
    LiteralBooleanArray* oldLiteralBooleanArray = dynamic_cast<LiteralBooleanArray*>(oldLiteral);
    LiteralFloatArray* oldLiteralFloatArray = dynamic_cast<LiteralFloatArray*>(oldLiteral);
    LiteralCharacterArray* oldLiteralCharacterArray = dynamic_cast<LiteralCharacterArray*>(oldLiteral);
    LiteralIntegerList* oldLiteralIntegerList = dynamic_cast<LiteralIntegerList*>(oldLiteral);
    LiteralBooleanList* oldLiteralBooleanList = dynamic_cast<LiteralBooleanList*>(oldLiteral);
    LiteralFloatList* oldLiteralFloatList = dynamic_cast<LiteralFloatList*>(oldLiteral);
    LiteralCharacterList* oldLiteralCharacterList = dynamic_cast<LiteralCharacterList*>(oldLiteral);

    aast::LocalDeclaration* newLocalDeclaration = new aast::LocalDeclaration();
    aast::IdentifierBeginningExpression* newIdentifierBeginningExpression = new aast::IdentifierBeginningExpression();
    aast::Call* newCall = new aast::Call();
    newIdentifierBeginningExpression->addAccessOrCall(newCall);
    newLocalDeclaration->setExpression(newIdentifierBeginningExpression);
    expression.addLiteralCreationStatement(newLocalDeclaration);

    if (oldLiteralInteger || oldLiteralBoolean || oldLiteralFloat || oldLiteralCharacter) {
        aast::Assignment* newAssignment = new aast::Assignment();
        // this = the first previous local declaration if it is anonymous
        aast::ThisBeginningExpression* thisAssignmentExpression = new aast::ThisBeginningExpression();
        aast::Access* accessForThis = new aast::Access();
        aast::IdentifierBeginningExpression* systemAssignmentExpression = new aast::IdentifierBeginningExpression();
        aast::Call* systemAssignmentExpressionCall = new aast::Call();
        accessForThis->setIdentifier("value");
        thisAssignmentExpression->addAccessOrCall(accessForThis);
        newAssignment->setExpression1(thisAssignmentExpression);
        systemAssignmentExpression->addAccessOrCall(systemAssignmentExpressionCall);
        newAssignment->setExpression2(systemAssignmentExpression);
        expression.addLiteralCreationStatement(newAssignment);

        if (oldLiteralInteger) {
            newIdentifierBeginningExpression->setIdentifier("Integer");
            systemAssignmentExpression->setIdentifier("systemAllocateInteger_"s + to_string(oldLiteralInteger->value));
        }

        else if (oldLiteralBoolean) {
            newIdentifierBeginningExpression->setIdentifier("Boolean");
            systemAssignmentExpression->setIdentifier("systemAllocateBoolean_"s + to_string(oldLiteralBoolean->value));
        }

        else if (oldLiteralFloat) {
            newIdentifierBeginningExpression->setIdentifier("Float");
            float integerPart;
            float fractionalPart = abs(modf(oldLiteralFloat->value, &integerPart));
            fractionalPart *= pow(10, 6);
            string identifier = "systemAllocateFloat_"s + to_string(static_cast<int>(integerPart)) + "_"s + to_string(static_cast<int>(fractionalPart));
            systemAssignmentExpression->setIdentifier(identifier);
        }

        else if (oldLiteralCharacter) {
            newIdentifierBeginningExpression->setIdentifier("Character");
            systemAssignmentExpression->setIdentifier("systemAllocateCharacter_"s + to_string(oldLiteralCharacter->value));
        }
    }

    else if (oldLiteralIntegerArray || oldLiteralBooleanArray || oldLiteralFloatArray || oldLiteralCharacterArray) {
        aast::IdentifierBeginningExpression* creationCallArgument = new aast::IdentifierBeginningExpression();
        aast::Call* creationCallArgumentCall = new aast::Call();
        aast::LiteralBeginningExpression* arraySize = new aast::LiteralBeginningExpression();
        creationCallArgumentCall->addExpression(arraySize);
        creationCallArgument->setIdentifier("Integer");
        creationCallArgument->addAccessOrCall(creationCallArgumentCall);
        newCall->addExpression(creationCallArgument);

        int oldArraySize = 0;
        if (oldLiteralIntegerArray) {
            newIdentifierBeginningExpression->setIdentifier("ArrayOfIntegers");
            LiteralIntegers* integers = oldLiteralIntegerArray->literalIntegers;
            do {
                aast::Assignment* nextInitializingAssignment = new aast::Assignment();
                aast::ThisBeginningExpression* thisForNextInitializingAssignment = new aast::ThisBeginningExpression();
                aast::Access* setAccess = new aast::Access();
                aast::Call* setCall = new aast::Call();
                aast::LiteralBeginningExpression* iteration = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*iteration, new LiteralInteger(oldArraySize));
                setCall->addExpression(iteration);
                setAccess->setIdentifier("set");
                thisForNextInitializingAssignment->addAccessOrCall(setAccess);
                thisForNextInitializingAssignment->addAccessOrCall(setCall);
                nextInitializingAssignment->setExpression1(thisForNextInitializingAssignment);

                aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*nextValue, new LiteralInteger(integers->literalInteger->value));
                nextInitializingAssignment->setExpression2(nextValue);

                oldArraySize++;
                integers = integers->literalIntegers;

                expression.addLiteralCreationStatement(nextInitializingAssignment);
            } while (integers);
        }

        else if (oldLiteralBooleanArray) {
            newIdentifierBeginningExpression->setIdentifier("ArrayOfBooleans");
            LiteralBooleans* booleans = oldLiteralBooleanArray->literalBooleans;
            do {
                aast::Assignment* nextInitializingAssignment = new aast::Assignment();
                aast::ThisBeginningExpression* thisForNextInitializingAssignment = new aast::ThisBeginningExpression();
                aast::Access* setAccess = new aast::Access();
                aast::Call* setCall = new aast::Call();
                aast::LiteralBeginningExpression* iteration = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*iteration, new LiteralInteger(oldArraySize));
                setCall->addExpression(iteration);
                setAccess->setIdentifier("set");
                thisForNextInitializingAssignment->addAccessOrCall(setAccess);
                thisForNextInitializingAssignment->addAccessOrCall(setCall);
                nextInitializingAssignment->setExpression1(thisForNextInitializingAssignment);

                aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*nextValue, new LiteralBoolean(booleans->literalBoolean->value));
                nextInitializingAssignment->setExpression2(nextValue);

                oldArraySize++;
                booleans = booleans->literalBooleans;

                expression.addLiteralCreationStatement(nextInitializingAssignment);
            } while (booleans);
        }

        else if (oldLiteralFloatArray) {
            newIdentifierBeginningExpression->setIdentifier("ArrayOfFloats");
            LiteralFloats* floats = oldLiteralFloatArray->literalFloats;
            do {
                aast::Assignment* nextInitializingAssignment = new aast::Assignment();
                aast::ThisBeginningExpression* thisForNextInitializingAssignment = new aast::ThisBeginningExpression();
                aast::Access* setAccess = new aast::Access();
                aast::Call* setCall = new aast::Call();
                aast::LiteralBeginningExpression* iteration = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*iteration, new LiteralInteger(oldArraySize));
                setCall->addExpression(iteration);
                setAccess->setIdentifier("set");
                thisForNextInitializingAssignment->addAccessOrCall(setAccess);
                thisForNextInitializingAssignment->addAccessOrCall(setCall);
                nextInitializingAssignment->setExpression1(thisForNextInitializingAssignment);

                aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*nextValue, new LiteralFloat(floats->literalFloat->value));
                nextInitializingAssignment->setExpression2(nextValue);

                oldArraySize++;
                floats = floats->literalFloats;

                expression.addLiteralCreationStatement(nextInitializingAssignment);
            } while (floats);
        }

        else if (oldLiteralCharacterArray) {
            newIdentifierBeginningExpression->setIdentifier("ArrayOfCharacters");
            LiteralCharacters* characters = oldLiteralCharacterArray->literalCharacters;
            do {
                aast::Assignment* nextInitializingAssignment = new aast::Assignment();
                aast::ThisBeginningExpression* thisForNextInitializingAssignment = new aast::ThisBeginningExpression();
                aast::Access* setAccess = new aast::Access();
                aast::Call* setCall = new aast::Call();
                aast::LiteralBeginningExpression* iteration = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*iteration, new LiteralInteger(oldArraySize));
                setCall->addExpression(iteration);
                setAccess->setIdentifier("set");
                thisForNextInitializingAssignment->addAccessOrCall(setAccess);
                thisForNextInitializingAssignment->addAccessOrCall(setCall);
                nextInitializingAssignment->setExpression1(thisForNextInitializingAssignment);

                aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
                populateLiteralCreationStatements(*nextValue, new LiteralCharacter(characters->literalCharacter->value));
                nextInitializingAssignment->setExpression2(nextValue);

                oldArraySize++;
                characters = characters->literalCharacters;

                expression.addLiteralCreationStatement(nextInitializingAssignment);
            } while (characters);
        }
        populateLiteralCreationStatements(*arraySize, new LiteralInteger(oldArraySize));
    }

    else if (oldLiteralIntegerList) {
        newIdentifierBeginningExpression->setIdentifier("ListOfIntegers");
        LiteralIntegers* integers = oldLiteralIntegerList->literalIntegers;
        do {
            aast::ThisBeginningExpression* thisForNextAppend = new aast::ThisBeginningExpression();
            aast::Access* appendAccess = new aast::Access();
            aast::Call* appendCall = new aast::Call();
            aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
            populateLiteralCreationStatements(*nextValue, new LiteralInteger(integers->literalInteger->value));
            appendCall->addExpression(nextValue);
            appendAccess->setIdentifier("append");
            thisForNextAppend->addAccessOrCall(appendAccess);
            thisForNextAppend->addAccessOrCall(appendCall);
            
            integers = integers->literalIntegers;

            expression.addLiteralCreationStatement(thisForNextAppend);
        } while (integers);
    }

    else if (oldLiteralBooleanList) {
        newIdentifierBeginningExpression->setIdentifier("ListOfBooleans");
        LiteralBooleans* booleans = oldLiteralBooleanList->literalBooleans;
        do {
            aast::ThisBeginningExpression* thisForNextAppend = new aast::ThisBeginningExpression();
            aast::Access* appendAccess = new aast::Access();
            aast::Call* appendCall = new aast::Call();
            aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
            populateLiteralCreationStatements(*nextValue, new LiteralBoolean(booleans->literalBoolean->value));
            appendCall->addExpression(nextValue);
            appendAccess->setIdentifier("append");
            thisForNextAppend->addAccessOrCall(appendAccess);
            thisForNextAppend->addAccessOrCall(appendCall);
            
            booleans = booleans->literalBooleans;

            expression.addLiteralCreationStatement(thisForNextAppend);
        } while (booleans);
    }

    else if (oldLiteralFloatList) {
        newIdentifierBeginningExpression->setIdentifier("ListOfFloats");
        LiteralFloats* floats = oldLiteralFloatList->literalFloats;
        do {
            aast::ThisBeginningExpression* thisForNextAppend = new aast::ThisBeginningExpression();
            aast::Access* appendAccess = new aast::Access();
            aast::Call* appendCall = new aast::Call();
            aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
            populateLiteralCreationStatements(*nextValue, new LiteralFloat(floats->literalFloat->value));
            appendCall->addExpression(nextValue);
            appendAccess->setIdentifier("append");
            thisForNextAppend->addAccessOrCall(appendAccess);
            thisForNextAppend->addAccessOrCall(appendCall);
            
            floats = floats->literalFloats;

            expression.addLiteralCreationStatement(thisForNextAppend);
        } while (floats);
    }

    else if (oldLiteralCharacterList) {
        newIdentifierBeginningExpression->setIdentifier("ListOfCharacters");
        LiteralCharacters* characters = oldLiteralCharacterList->literalCharacters;
        do {
            aast::ThisBeginningExpression* thisForNextAppend = new aast::ThisBeginningExpression();
            aast::Access* appendAccess = new aast::Access();
            aast::Call* appendCall = new aast::Call();
            aast::LiteralBeginningExpression* nextValue = new aast::LiteralBeginningExpression();
            populateLiteralCreationStatements(*nextValue, new LiteralCharacter(characters->literalCharacter->value));
            appendCall->addExpression(nextValue);
            appendAccess->setIdentifier("append");
            thisForNextAppend->addAccessOrCall(appendAccess);
            thisForNextAppend->addAccessOrCall(appendCall);
            
            characters = characters->literalCharacters;

            expression.addLiteralCreationStatement(thisForNextAppend);
        } while (characters);
    }
}


aast::Entity* SemanticalAnalyzer::mapToAAST(Entity* oldVertex) {
    Program* oldProgram = dynamic_cast<Program*>(oldVertex);
    ClassDeclaration* oldClassDeclaration = dynamic_cast<ClassDeclaration*>(oldVertex);
    ClassBody* oldClassBody = dynamic_cast<ClassBody*>(oldVertex);
    MethodDeclaration* oldMethodDeclaration = dynamic_cast<MethodDeclaration*>(oldVertex);
    AnyBody* oldAnyBody = dynamic_cast<AnyBody*>(oldVertex);
    ParameterDeclaration* oldParameterDeclaration = dynamic_cast<ParameterDeclaration*>(oldVertex);
    LocalDeclaration* oldLocalDeclaration = dynamic_cast<LocalDeclaration*>(oldVertex);
    IdBeginningExpression* oldIdentifierBeginningExpression = dynamic_cast<IdBeginningExpression*>(oldVertex);
    ThisBeginningExpression* oldThisBeginningExpression = dynamic_cast<ThisBeginningExpression*>(oldVertex);
    BaseBeginningExpression* oldBaseBeginningExpression = dynamic_cast<BaseBeginningExpression*>(oldVertex);
    LiteralBeginningExpression* oldLiteralBeginningExpression = dynamic_cast<LiteralBeginningExpression*>(oldVertex);
    Assignment* oldAssignment = dynamic_cast<Assignment*>(oldVertex);
    IfStatement* oldIfStatement = dynamic_cast<IfStatement*>(oldVertex);
    WhileStatement* oldWhileStatement = dynamic_cast<WhileStatement*>(oldVertex);
    ReturnStatement* oldReturnStatement = dynamic_cast<ReturnStatement*>(oldVertex);
    ConstructorDeclaration* oldConstructorDeclaration = dynamic_cast<ConstructorDeclaration*>(oldVertex);
    Declaration* oldDeclaration = dynamic_cast<Declaration*>(oldVertex);

    if (oldProgram) {
        aast::Program* newProgram = new aast::Program();
        ClassDeclarations* oldDeclarations = oldProgram->classDeclarations;
        do {
            if (!oldDeclarations->classDeclaration) {
                oldDeclarations = oldDeclarations->classDeclarations;
                continue;
            }
            aast::ClassDeclaration* newDeclaration = dynamic_cast<aast::ClassDeclaration*>(this->mapToAAST(oldDeclarations->classDeclaration));
            newProgram->addClassDeclaration(newDeclaration);
            oldDeclarations = oldDeclarations->classDeclarations;
        } while (oldDeclarations);
        this->tree = newProgram;
        return newProgram;
    }

    else if (oldClassDeclaration) {
        aast::ClassDeclaration* newClassDeclaration = new aast::ClassDeclaration();
        newClassDeclaration->setClassName(oldClassDeclaration->classHead->name->identifier);
        if (oldClassDeclaration->classHead->baseName)
            newClassDeclaration->setClassBaseName(oldClassDeclaration->classHead->baseName->identifier);
        else
            newClassDeclaration->setClassBaseName("");
        aast::ClassBody* newBody = dynamic_cast<aast::ClassBody*>(this->mapToAAST(oldClassDeclaration->classBody));
        newClassDeclaration->setClassBody(newBody);
        return newClassDeclaration;
    }

    else if (oldClassBody) {
        aast::ClassBody* newClassBody = new aast::ClassBody();
        ClassStatements* oldClassStatements = oldClassBody->classStatements;
        do {
            if (!oldClassStatements->classStatement) {
                oldClassStatements = oldClassStatements->classStatements;
                continue;
            }
            aast::ClassStatement* newClassStatement = dynamic_cast<aast::ClassStatement*>(this->mapToAAST(oldClassStatements->classStatement));
            newClassBody->addClassStatement(newClassStatement);
            oldClassStatements = oldClassStatements->classStatements;
        } while (oldClassStatements);
        return newClassBody;
    }

    else if (oldMethodDeclaration) {
        aast::MethodDeclaration* newMethodDeclaration = new aast::MethodDeclaration();
        newMethodDeclaration->setName(oldMethodDeclaration->methodHead->name->identifier);
        if (oldMethodDeclaration->methodHead->returnType)
            newMethodDeclaration->setType(oldMethodDeclaration->methodHead->returnType->identifier);
        else
            newMethodDeclaration->setType("");
        aast::AnyBody* newAnyBody = dynamic_cast<aast::AnyBody*>(this->mapToAAST(oldMethodDeclaration->body));
        newMethodDeclaration->setAnyBody(newAnyBody);
        ParameterDeclarations* oldParameterDeclarations = oldMethodDeclaration->methodHead->parameterDeclarations;
        do {
            if (!oldParameterDeclarations->parameterDeclaration) {
                oldParameterDeclarations = oldParameterDeclarations->parameterDeclarations;
                continue;
            }
            aast::ParameterDeclaration* newParameterDeclaration = dynamic_cast<aast::ParameterDeclaration*>(this->mapToAAST(oldParameterDeclarations->parameterDeclaration));
            newMethodDeclaration->addParameterDeclaration(newParameterDeclaration);
            oldParameterDeclarations = oldParameterDeclarations->parameterDeclarations;
        } while (oldParameterDeclarations);
        return newMethodDeclaration;
    }

    else if (oldAnyBody) {
        aast::AnyBody* newAnyBody = new aast::AnyBody();
        Statements* oldStatements = oldAnyBody->statements;
        do {
            if (!oldStatements->statement) {
                oldStatements = oldStatements->statements;
                continue;
            }
            aast::Statement* newStatement = dynamic_cast<aast::Statement*>(this->mapToAAST(oldStatements->statement));
            newAnyBody->addStatement(newStatement);
            oldStatements = oldStatements->statements;
        } while (oldStatements);
        return newAnyBody;
    }

    else if (oldParameterDeclaration) {
        aast::ParameterDeclaration* newParameterDeclaration = new aast::ParameterDeclaration();
        newParameterDeclaration->setName(oldParameterDeclaration->name->identifier);
        newParameterDeclaration->setType(oldParameterDeclaration->identifier->identifier);
        return newParameterDeclaration;
    }

    else if (oldLocalDeclaration) {
        aast::LocalDeclaration* newLocalDeclaration = new aast::LocalDeclaration();
        newLocalDeclaration->setName(oldLocalDeclaration->name->identifier);
        aast::Expression* newExpression = dynamic_cast<aast::Expression*>(this->mapToAAST(oldLocalDeclaration->expression));
        newLocalDeclaration->setExpression(newExpression);
        return newLocalDeclaration;
    }

    else if (oldIdentifierBeginningExpression) {
        aast::IdentifierBeginningExpression* newIdentifierBeginningExpression = new aast::IdentifierBeginningExpression();
        newIdentifierBeginningExpression->setIdentifier(oldIdentifierBeginningExpression->identifier->identifier);
        AccessesAndCalls* oldAccessesAndCalls = oldIdentifierBeginningExpression->accessesAndCalls;
        do {
            if (dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls)) {
                AccessAndFurther* oldAccessAndFurther = dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls);
                aast::Access* newAccess = new aast::Access();
                newAccess->setIdentifier(oldAccessAndFurther->identifier->identifier);
                newIdentifierBeginningExpression->addAccessOrCall(newAccess);
                oldAccessesAndCalls = oldAccessAndFurther->accessesAndCalls;
            }
            else if (oldAccessesAndCalls) {
                CallAndFurther* oldCallAndFurther = dynamic_cast<CallAndFurther*>(oldAccessesAndCalls);
                aast::Call* newCall = new aast::Call();
                Expressions* oldExpressions = oldCallAndFurther->expressions;
                do {
                    if (!oldExpressions->expression) {
                        oldExpressions = oldExpressions->expressions;
                        continue;
                    }
                    newCall->addExpression(dynamic_cast<aast::Expression*>(this->mapToAAST(oldExpressions->expression)));
                    oldExpressions = oldExpressions->expressions;
                } while (oldExpressions);
                newIdentifierBeginningExpression->addAccessOrCall(newCall);
                oldAccessesAndCalls = oldCallAndFurther->accessesAndCalls;
            }
        } while (oldAccessesAndCalls);
        return newIdentifierBeginningExpression;
    }
    
    else if (oldThisBeginningExpression) {
        aast::ThisBeginningExpression* newThisBeginningExpression = new aast::ThisBeginningExpression();
        AccessesAndCalls* oldAccessesAndCalls = oldThisBeginningExpression->accessesAndCalls;
        do {
            if (dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls)) {
                AccessAndFurther* oldAccessAndFurther = dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls);
                aast::Access* newAccess = new aast::Access();
                newAccess->setIdentifier(oldAccessAndFurther->identifier->identifier);
                newThisBeginningExpression->addAccessOrCall(newAccess);
                oldAccessesAndCalls = oldAccessAndFurther->accessesAndCalls;
            }
            else if (oldAccessesAndCalls) {
                CallAndFurther* oldCallAndFurther = dynamic_cast<CallAndFurther*>(oldAccessesAndCalls);
                aast::Call* newCall = new aast::Call();
                Expressions* oldExpressions = oldCallAndFurther->expressions;
                do {
                    if (!oldExpressions->expression) {
                        oldExpressions = oldExpressions->expressions;
                        continue;
                    }
                    newCall->addExpression(dynamic_cast<aast::Expression*>(this->mapToAAST(oldExpressions->expression)));
                    oldExpressions = oldExpressions->expressions;
                } while (oldExpressions);
                newThisBeginningExpression->addAccessOrCall(newCall);
                oldAccessesAndCalls = oldCallAndFurther->accessesAndCalls;
            }
        } while (oldAccessesAndCalls);
        return newThisBeginningExpression;
    }

    else if (oldBaseBeginningExpression) {
        aast::BaseBeginningExpression* newBaseBeginningExpression = new aast::BaseBeginningExpression();
        AccessesAndCalls* oldAccessesAndCalls = oldBaseBeginningExpression->accessesAndCalls;
        do {
            if (dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls)) {
                AccessAndFurther* oldAccessAndFurther = dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls);
                aast::Access* newAccess = new aast::Access();
                newAccess->setIdentifier(oldAccessAndFurther->identifier->identifier);
                newBaseBeginningExpression->addAccessOrCall(newAccess);
                oldAccessesAndCalls = oldAccessAndFurther->accessesAndCalls;
            }
            else if (oldAccessesAndCalls) {
                CallAndFurther* oldCallAndFurther = dynamic_cast<CallAndFurther*>(oldAccessesAndCalls);
                aast::Call* newCall = new aast::Call();
                Expressions* oldExpressions = oldCallAndFurther->expressions;
                do {
                    if (!oldExpressions->expression) {
                        oldExpressions = oldExpressions->expressions;
                        continue;
                    }
                    newCall->addExpression(dynamic_cast<aast::Expression*>(this->mapToAAST(oldExpressions->expression)));
                    oldExpressions = oldExpressions->expressions;
                } while (oldExpressions);
                newBaseBeginningExpression->addAccessOrCall(newCall);
                oldAccessesAndCalls = oldCallAndFurther->accessesAndCalls;
            }
        } while (oldAccessesAndCalls);
        return newBaseBeginningExpression;
    }

    else if (oldLiteralBeginningExpression) {
        aast::LiteralBeginningExpression* newLiteralBeginningExpression = new aast::LiteralBeginningExpression();
        populateLiteralCreationStatements(*newLiteralBeginningExpression, oldLiteralBeginningExpression->literal);
        AccessesAndCalls* oldAccessesAndCalls = oldLiteralBeginningExpression->accessesAndCalls;
        do {
            if (dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls)) {
                AccessAndFurther* oldAccessAndFurther = dynamic_cast<AccessAndFurther*>(oldAccessesAndCalls);
                aast::Access* newAccess = new aast::Access();
                newAccess->setIdentifier(oldAccessAndFurther->identifier->identifier);
                newLiteralBeginningExpression->addAccessOrCall(newAccess);
                oldAccessesAndCalls = oldAccessAndFurther->accessesAndCalls;
            }
            else if (oldAccessesAndCalls) {
                CallAndFurther* oldCallAndFurther = dynamic_cast<CallAndFurther*>(oldAccessesAndCalls);
                aast::Call* newCall = new aast::Call();
                Expressions* oldExpressions = oldCallAndFurther->expressions;
                do {
                    if (!oldExpressions->expression) {
                        oldExpressions = oldExpressions->expressions;
                        continue;
                    }
                    newCall->addExpression(dynamic_cast<aast::Expression*>(this->mapToAAST(oldExpressions->expression)));
                    oldExpressions = oldExpressions->expressions;
                } while (oldExpressions);
                newLiteralBeginningExpression->addAccessOrCall(newCall);
                oldAccessesAndCalls = oldCallAndFurther->accessesAndCalls;
            }
        } while (oldAccessesAndCalls);
        return newLiteralBeginningExpression;
    }

    else if (oldAssignment) {
        aast::Assignment* newAssignment = new aast::Assignment();
        newAssignment->setExpression1(dynamic_cast<aast::Expression*>(this->mapToAAST(oldAssignment->expression1)));
        newAssignment->setExpression2(dynamic_cast<aast::Expression*>(this->mapToAAST(oldAssignment->expression2)));
        return newAssignment;
    }

    else if (oldIfStatement) {
        aast::IfStatement* newIfStatement = new aast::IfStatement();
        newIfStatement->setCondition(dynamic_cast<aast::Expression*>(this->mapToAAST(oldIfStatement->condition)));
        Statements* oldTrueStatements = oldIfStatement->trueStatements;
        do {
            newIfStatement->addTrueStatement(dynamic_cast<aast::Expression*>(this->mapToAAST(oldTrueStatements->statement)));
            oldTrueStatements = oldTrueStatements->statements;
        } while (oldTrueStatements);
        Statements* oldFalseStatements = oldIfStatement->falseStatements;
        if (oldFalseStatements)
        do {
            newIfStatement->addFalseStatement(dynamic_cast<aast::Expression*>(this->mapToAAST(oldFalseStatements->statement)));
            oldFalseStatements = oldFalseStatements->statements;
        } while (oldFalseStatements);
        return newIfStatement;
    }

    else if (oldWhileStatement) {
        aast::WhileStatement* newWhileStatement = new aast::WhileStatement();
        newWhileStatement->setCondition(dynamic_cast<aast::Expression*>(this->mapToAAST(oldWhileStatement->condition)));
        Statements* oldStatements = oldWhileStatement->statements;
        do {
            newWhileStatement->addStatement(dynamic_cast<aast::Expression*>(this->mapToAAST(oldStatements->statement)));
            oldStatements = oldStatements->statements;
        } while (oldStatements);
        return newWhileStatement;
    }

    else if (oldReturnStatement) {
        aast::ReturnStatement* newReturnStatement = new aast::ReturnStatement();
        newReturnStatement->setExpression(dynamic_cast<aast::Expression*>(this->mapToAAST(oldReturnStatement->expression)));
        return newReturnStatement;
    }

    else if (oldConstructorDeclaration) {
        aast::ConstructorDeclaration* newConstructorDeclaration = new aast::ConstructorDeclaration();
        newConstructorDeclaration->setAnyBody(dynamic_cast<aast::AnyBody*>(this->mapToAAST(oldConstructorDeclaration->body)));
        ParameterDeclarations* oldParameterDeclarations = oldConstructorDeclaration->constructorHead->parameterDeclarations;
        do {
            if (!oldParameterDeclarations->parameterDeclaration) {
                oldParameterDeclarations = oldParameterDeclarations->parameterDeclarations;
                continue;
            }
            newConstructorDeclaration->addParameterDeclaration(dynamic_cast<aast::ParameterDeclaration*>(this->mapToAAST(oldParameterDeclarations->parameterDeclaration)));
            oldParameterDeclarations = oldParameterDeclarations->parameterDeclarations;
        } while (oldParameterDeclarations);
        return newConstructorDeclaration;
    }

    else if (oldDeclaration) {
        aast::Declaration* newDeclaration = new aast::Declaration();
        newDeclaration->setName(oldDeclaration->name->identifier);
        newDeclaration->setExpression(dynamic_cast<aast::Expression*>(this->mapToAAST(oldDeclaration->expression)));
        return newDeclaration;
    }
}
