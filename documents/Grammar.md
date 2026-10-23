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
&nbsp;&nbsp;&nbsp;&nbsp;: **ClassHead** **Body**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ClassHead**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenClass* **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenClass* **Id** *TokenColon* **Id**  
&nbsp;&nbsp;&nbsp;&nbsp;;

> Other alternatives are either included to their non-terminal classes with nullptr's or represented by additional non-terminals, so their predecessor is just used for polymorphic behavior and does not contain anything but a parse function for the alternatives

**Id**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenIdentifier*  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Body**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenIs* **Statements** *TokenEnd*  
&nbsp;&nbsp;&nbsp;&nbsp;| *TokenArrow* **Statement**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Statements**  
&nbsp;&nbsp;&nbsp;&nbsp;: **Statement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Statement Statements**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**Statement**  
&nbsp;&nbsp;&nbsp;&nbsp;: **MethodDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ConstructorDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **LocalDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Assignment**  
&nbsp;&nbsp;&nbsp;&nbsp;| **IfStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **WhileStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **ReturnStatement**  
&nbsp;&nbsp;&nbsp;&nbsp;| **Expression**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**MethodDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: **MethodHead Body**  
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

**ConstructorDeclaration**  
&nbsp;&nbsp;&nbsp;&nbsp;: **ConstructorHead Body**  
&nbsp;&nbsp;&nbsp;&nbsp;;

**ConstructorHead**  
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenThis TokenOpeningParenthesis* **ParameterDeclarations** *TokenClosingParenthesis*  
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
&nbsp;&nbsp;&nbsp;&nbsp;: *TokenCharacter*  
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