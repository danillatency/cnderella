> Each non-terminal is a class

**Program**  
&nbsp;&nbsp;&nbsp;&nbsp;: **ClassDeclarations**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ClassDeclarations**  
&nbsp;&nbsp;&nbsp;&nbsp;: **ClassDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ClassDeclaration ClassDeclarations**  
&nbsp;&nbsp;&nbsp;&nbsp;;

> Alternatives representing lists are included to their non-terminal classes directly (the second field may be nullptr)

**ClassDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: **ClassHead** **ClassBody**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ClassHead**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenClass* **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenClass* **Id** *TokenColon* **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;;

> Other alternatives are either included to their non-terminal classes with nullptr's or represented by additional non-terminals, so their predecessor is just used for polymorphic behavior and does not contain anything but a parse function for the alternatives

**Id**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenIdentifier*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ClassBody**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenIs* **ClassStatements** *TokenEnd*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenArrow* **ClassStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ClassStatements**  
&nbsp;&nbsp;&nbsp;&nbsp;: **ClassStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ClassStatement ClassStatements**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ClassStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;: **MethodDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ConstructorDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Declaration**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**MethodDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: **MethodHead AnyBody**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**MethodHead**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenMethod* **Id** *TokenOpeningParenthesis* **ParameterDeclarations** *TokenClosingParenthesis*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenMethod* **Id** *TokenOpeningParenthesis* **ParameterDeclarations** *TokenClosingParenthesis TokenColon* **Identifier**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ParameterDeclarations**  
&nbsp;&nbsp;&nbsp;&nbsp;:  
&nbsp;&nbsp;&nbsp;&nbsp;| **ParameterDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ParameterDeclaration** *TokenComma* **ParameterDeclarations**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ParameterDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: **Id** *TokenColon* **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**AnyBody**
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenIs* **Statements** *TokenEnd*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenArrow* **Statement**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Statements**  
&nbsp;&nbsp;&nbsp;&nbsp;: **Statement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Statement Statements**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Statement**  
&nbsp;&nbsp;&nbsp;&nbsp;: **LocalDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Assignment**  
&nbsp;&nbsp;&nbsp;&nbsp;| **IfStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **WhileStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ReturnStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LocalDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenVar* **Id** *TokenAssignment* **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;: **IdBeginningExpression**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ThisBeginningExpression**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralBeginningExpression**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**IdBeginningExpression**  
&nbsp;&nbsp;&nbsp;&nbsp;: **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Id** **AccessesAndCalls**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**AccessesAndCalls**  
&nbsp;&nbsp;&nbsp;&nbsp;: **AccessAndFurther**  
&nbsp;&nbsp;&nbsp;&nbsp;| **CallAndFurther**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**AccessAndFurther**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenDot* **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenDot* **Id** **AccessesAndCalls**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**CallAndFurther**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningParenthesis* **Expressions** *TokenClosingParenthesis*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenOpeningParenthesis* **Expressions** *TokenClosingParenthesis* **AccessesAndCalls**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Expressions**  
&nbsp;&nbsp;&nbsp;&nbsp;:  
&nbsp;&nbsp;&nbsp;&nbsp;| **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Expression** *TokenComma* **Expressions**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ThisBeginningExpression**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenThis*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenThis* **AccessesAndCalls**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralBeginningExpression**  
&nbsp;&nbsp;&nbsp;&nbsp;: **Literal**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Literal** **AccessesAndCalls**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Literal**  
&nbsp;&nbsp;&nbsp;&nbsp;: **LiteralInteger**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralBoolean**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralFloat**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralCharacter**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralIntegerArray**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralBooleanArray**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralFloatArray**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralCharacterArray**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralIntegerList**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralBooleanList**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralFloatList**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralCharacterList**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralInteger**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenInteger*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralBoolean**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenBoolean*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralFloat**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenFloat*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralCharacter**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenQuote TokenCharacter TokenQuote*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralIntegerArray**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningSquareBracket* **LiteralIntegers** *TokenClosingSquareBracket*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralIntegers**  
&nbsp;&nbsp;&nbsp;&nbsp;: **LiteralInteger**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralInteger** *TokenComma* **LiteralIntegers**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralBooleanArray**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningSquareBracket* **LiteralBooleans** *TokenClosingSquareBracket*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralBooleans**  
&nbsp;&nbsp;&nbsp;&nbsp;: **LiteralBoolean**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralBoolean** *TokenComma* **LiteralBooleans**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralFloatArray**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningSquareBracket* **LiteralFloats** *TokenClosingSquareBracket*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralFloats**  
&nbsp;&nbsp;&nbsp;&nbsp;: **LiteralFloat**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralFloat** *TokenComma* **LiteralFloats**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralCharacterArray**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningSquareBracket* **LiteralCharacters** *TokenClosingSquareBracket*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenDoubleQuote* **LiteralCharacters** *TokenDoubleQuote*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralCharacters**  
&nbsp;&nbsp;&nbsp;&nbsp;: **LiteralCharacter**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralCharacter LiteralCharacters**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LiteralCharacter** *TokenComma* **LiteralCharacters**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralIntegerList**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningCurlyBrace* **LiteralIntegers** *TokenClosingCurlyBrace*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralBooleanList**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningCurlyBrace* **LiteralBooleans** *TokenClosingCurlyBrace*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralFloatList**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningCurlyBrace* **LiteralFloats** *TokenClosingCurlyBrace*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**LiteralCharacterList**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenOpeningCurlyBrace* **LiteralCharacters** *TokenClosingCurlyBrace*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Assignment**  
&nbsp;&nbsp;&nbsp;&nbsp;: **Expression** *TokenAssignment* **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**IfStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenIf* **Expression** *TokenThen* **Statements** *TokenEnd*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenIf* **Expression** *TokenThen* **Statements** *TokenElse* **Statements** *TokenEnd*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**WhileStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenWhile* **Expression** *TokenLoop* **Statements** *TokenEnd*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ReturnStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenReturn* **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ConstructorDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: **ConstructorHead AnyBody**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ConstructorHead**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenThis TokenOpeningParenthesis* **ParameterDeclarations** *TokenClosingParenthesis*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Declaration**
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenVar* **Id** *TokenAssignment* **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;;
