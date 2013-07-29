#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <list>
#include "bad_jaba.h"
#include "lexer.h"

using std::clog;
using std::endl;
using std::list;

class Parser{
    list<Token> token_buf;
    list<Token>::iterator next_token;
    list<Token>::iterator cur_token;
//    Token prev_token;
//    Token cur_token;
//    Token next_token;
    int stored_tokens;
    bool next_available;
    bool recall_available;
    int cur_n, saved_n;
    void recall(list<Token>::iterator save);

    Lexer *lexer;

    bool cur_is_of(token_type type, bool load_next = true);
    bool Program(){list<Token>::iterator save = cur_token;
                return Program1() || (recall(save), Program2());}
    bool Program2(){return Element() && Program();}
    bool Program1(){return cur_is_of(ENDOF, false);}
    bool Element(){list<Token>::iterator save = cur_token;
                return Element1() || (recall(save), Element2());}
    bool Element1();
    bool Element2(){return Statement();}
    bool ParameterListOpt(){list<Token>::iterator save = cur_token;
                return ParameterListOpt1() || (recall(save), ParameterListOpt2());}
    bool ParameterListOpt2(){return ParameterList();}
    bool ParameterListOpt1(){return cur_is_of(CLOSE_PAR, false);}
    bool ParameterList(){list<Token>::iterator save = cur_token;
                return ParameterList1() || (recall(save), ParameterList2());}
    bool ParameterList1(){return Ident();}
    bool ParameterList2();
    bool CompoundStatement();
    bool Statements(){list<Token>::iterator save = cur_token;
                return Statements1() || (recall(save), Statements2());}
    bool Statements2(){return Statement() && Statements();}
    bool Statements1(){return cur_is_of(CLOSE_BRACE, false);}
    bool Statement(){list<Token>::iterator save = cur_token;
                return Statement1() || (recall(save), Statement2()) || (recall(save), Statement3()) ||
                          (recall(save), Statement4()) || (recall(save), Statement5()) || (recall(save), Statement6()) ||
                          (recall(save), Statement7()) || (recall(save), Statement8()) || (recall(save), Statement9()) ;}
    bool Statement1();
    bool Statement2();
    bool Statement3();
    bool Statement4();
    bool Statement5();
    bool Statement6();
    bool Statement7();
    bool Statement8(){return CompoundStatement();}
    bool Statement9();
    bool Condition();
    bool ElseNone(){list<Token>::iterator save = cur_token;
                return ElseNone1() || (recall(save), ElseNone2());}
    bool ElseNone1();
    bool ElseNone2(){return true;}
    bool ForParen();
    bool VariablesOrExpression(){list<Token>::iterator save = cur_token;
                return VariablesOrExpression1() || (recall(save), VariablesOrExpression2());}
    bool VariablesOrExpression1();
    bool VariablesOrExpression2(){return Expression();}
    bool Variables(){list<Token>::iterator save = cur_token;
                return Variables1() || (recall(save), Variables2());}
    bool Variables1(){return Variable();}
    bool Variables2();
    bool Variable(){list<Token>::iterator save = cur_token;
                return Variable1() || (recall(save), Variable2());}
    bool Variable1(){return Ident();}
    bool Variable2();
    bool ExpressionOpt(){list<Token>::iterator save = cur_token;
                return ExpressionOpt1() || (recall(save), ExpressionOpt2());}
    bool ExpressionOpt2(){return Expression();}
    bool ExpressionOpt1(){return cur_is_of(SEMICOLON, false);}
    bool Expression(){list<Token>::iterator save = cur_token;
                return Expression1() || (recall(save), Expression2());}
    bool Expression1(){return AssignmentExpression();}
    bool Expression2();
    bool AssignmentExpression(){list<Token>::iterator save = cur_token;
                return AssignmentExpression1() || (recall(save), AssignmentExpression2());}
    bool AssignmentExpression2(){return ConditionalExpression();}
    bool AssignmentExpression1();
    bool ConditionalExpression(){list<Token>::iterator save = cur_token;
                return ConditionalExpression1() || (recall(save), ConditionalExpression2());}
    bool ConditionalExpression1(){return OrExpression();}
    bool ConditionalExpression2();
    bool OrExpression(){list<Token>::iterator save = cur_token;
                return OrExpression1() || (recall(save), OrExpression2());}
    bool OrExpression1(){return AndExpression();}
    bool OrExpression2();
    bool AndExpression(){list<Token>::iterator save = cur_token;
                return AndExpression1() || (recall(save), AndExpression2());}
    bool AndExpression1(){return EqualityExpression();}
    bool AndExpression2();
    bool EqualityExpression(){list<Token>::iterator save = cur_token;
                return EqualityExpression1() || (recall(save), EqualityExpression2());}
    bool EqualityExpression1(){return RelationalExpression();}
    bool EqualityExpression2();
    bool RelationalExpression(){list<Token>::iterator save = cur_token;
                return RelationalExpression1() || (recall(save), RelationalExpression2());}
    bool RelationalExpression1(){return AdditiveExpression();}
    bool RelationalExpression2();
    bool AdditiveExpression(){list<Token>::iterator save = cur_token;
                return AdditiveExpression1() ||
            (recall(save), AdditiveExpression2()) || (recall(save), AdditiveExpression3());}
    bool AdditiveExpression1(){return MultiplicativeExpression();}
    bool AdditiveExpression2();
    bool AdditiveExpression3();
    bool MultiplicativeExpression(){list<Token>::iterator save = cur_token;
                return MultiplicativeExpression1() || (recall(save), MultiplicativeExpression2());}
    bool MultiplicativeExpression1(){return UnaryExpression();}
    bool MultiplicativeExpression2();
    bool UnaryExpression(){list<Token>::iterator save = cur_token;
                return UnaryExpression1() ||
            (recall(save), UnaryExpression2()) || (recall(save), UnaryExpression3()) ||
            (recall(save), UnaryExpression4()) || (recall(save), UnaryExpression5());}
    bool UnaryExpression1(){return MemberExpression();}
    bool UnaryExpression2();
    bool UnaryExpression3();
    bool UnaryExpression4(){return IncrementOperator() && MemberExpression();}
    bool UnaryExpression5(){return MemberExpression() && IncrementOperator();}
    bool MemberExpression(){list<Token>::iterator save = cur_token;
                return MemberExpression1() ||
            (recall(save), MemberExpression2()) || (recall(save), MemberExpression3()) ||
            (recall(save), MemberExpression4());}
    bool MemberExpression1(){return PrimaryExpression();}
    bool MemberExpression2();
    bool MemberExpression3();
    bool MemberExpression4();
    bool ArgumentListOpt(){list<Token>::iterator save = cur_token;
                return ArgumentListOpt1() || (recall(save), ArgumentListOpt2());}
    bool ArgumentListOpt2(){return ArgumentList();}
    bool ArgumentListOpt1(){return cur_is_of(CLOSE_PAR, false);}
    bool ArgumentList(){list<Token>::iterator save = cur_token;
                return ArgumentList1() || (recall(save), ArgumentList2());}
    bool ArgumentList1(){return AssignmentExpression();}
    bool ArgumentList2();
    bool PrimaryExpression(){list<Token>::iterator save = cur_token;
                return PrimaryExpression1() ||
            (recall(save), PrimaryExpression2()) || (recall(save), PrimaryExpression3()) ||
            (recall(save), PrimaryExpression4()) || (recall(save), PrimaryExpression5()) ||
            (recall(save), PrimaryExpression6()) || (recall(save), PrimaryExpression7()) ||
            (recall(save), PrimaryExpression8());}
    bool PrimaryExpression1();
    bool PrimaryExpression2(){return Ident();}
    bool PrimaryExpression3();
    bool PrimaryExpression4();
    bool PrimaryExpression5();
    bool PrimaryExpression6();
    bool PrimaryExpression7();
    bool PrimaryExpression8();
    bool Ident();
    bool UnaryOperator();
    bool IncrementOperator();
    bool MultiplicativeOperator();
    bool RelationalOperator();
    bool EqualityualityOperator();
    bool AssignmentOperator();


public:
    Parser(Lexer *used_lexer): lexer(used_lexer) {}
    ~Parser(){}
    void parse();
};

#endif // PARSER_H
