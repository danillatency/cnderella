#ifndef nonterminalerrorsheader
#define nonterminalerrorsheader

#include <stdexcept>

#include "../lexical_analyzer/Tokens.h"


using namespace std;


namespace parser {
	class ClassDeclarationExpected : public domain_error {
	public:
		ClassDeclarationExpected(Span span);
	};


	class OutOfTokens : public domain_error {
	public:
		OutOfTokens(Span span);
	};


    class EnumerationEnd : public domain_error {
    public:
        EnumerationEnd(Span span);
    };


	class IdentifierExpected : public domain_error {
	public:
		IdentifierExpected(Span span);
	};


	class EndExpected : public domain_error {
	public:
		EndExpected(string context, Span span);
	};


    class NotLiteral : public domain_error {
    public:
        NotLiteral(Span span);
    };


    class FailedToParseLiteral : public domain_error {
    public:
        FailedToParseLiteral(string context, Span span);
    };


	class ArrowExpected : public domain_error {
	public:
        ArrowExpected(string context, Span span);
	};


    class MethodExpected : public domain_error {
    public:
        MethodExpected(Span span);
    };


    class OpeningParenthesisExpected : public domain_error {
    public:
        OpeningParenthesisExpected(Span span);
    };


    class ClosingParenthesisExpected : public domain_error {
    public:
        ClosingParenthesisExpected(string context, Span span);
    };


    class CommaExpected : public domain_error {
    public:
        CommaExpected(Span span);
    };


    class ColonExpected : public domain_error {
    public:
        ColonExpected(Span span);
    };


    class VarExpected : public domain_error {
    public:
        VarExpected(string context, Span span);
    };


    class DotExpected : public domain_error {
    public:
        DotExpected(Span span);
    };


    class ThisExpected : public domain_error {
    public:
        ThisExpected(string context, Span span);
    };


    class BaseExpected : public domain_error {
    public:
        BaseExpected(Span span);
    };


    class FailedToParse : public domain_error {
    public:
        FailedToParse(Span span);
    };


    class QuoteExpected : public domain_error {
    public:
        QuoteExpected(Span span);
    };


    class OpeningSquareBracketExpected : public domain_error {
    public:
        OpeningSquareBracketExpected(Span span);
    };


    class ClosingSquareBracketExpected : public domain_error {
    public:
        ClosingSquareBracketExpected(string context, Span span);
    };


    class OpeningSquareBracketOrQuotesExpected : public domain_error {
    public:
        OpeningSquareBracketOrQuotesExpected(Span span);
    };


    class ClosingSquareBracketOrQuotesExpected : public domain_error {
    public:
        ClosingSquareBracketOrQuotesExpected(Span span);
    };


    class OpeningCurlyBraceExpected : public domain_error {
    public:
        OpeningCurlyBraceExpected(string context, Span span);
    };


    class ClosingCurlyBraceExpected : public domain_error {
    public:
        ClosingCurlyBraceExpected(string context, Span span);
    };


    class AssignmentSignExpected : public domain_error {
    public:
        AssignmentSignExpected(string context, Span span);
    };


    class IfExpected : public domain_error {
    public:
        IfExpected(Span span);
    };


    class ThenExpected : public domain_error {
    public:
        ThenExpected(Span span);
    };


    class ElseExpected : public domain_error {
    public:
        ElseExpected(Span span);
    };


    class WhileExpected : public domain_error {
    public:
        WhileExpected(Span span);
    };


    class LoopExpected : public domain_error {
    public:
        LoopExpected(Span span);
    };


    class ReturnExpected : public domain_error {
    public:
        ReturnExpected(Span span);
    };


    class VarMethodOrThisExpected : public domain_error {
    public:
        VarMethodOrThisExpected(Span span);
    };


    class VarIfWhileReturnIdentifier : public domain_error {
    public:
        VarIfWhileReturnIdentifier(Span span);
    };


    class RuleParseError : public domain_error {
    public:
        RuleParseError(string element, Span span, string reason);
    };
}


#endif
