#include "parser.h"

void Parser::recall(list<Token>::iterator save){
//    if(recall_available){
//        next_available = cur_token != prev_token;
//        if(next_available)  next_token = cur_token;
//        cur_token = prev_token;
//    } else throw bad_jaba(E_PARSER, "recall demanded while no token to recall to");
    list<Token>::iterator tmp = cur_token;
    cur_token = save;
    if((save != tmp) && (++save != tmp))
        throw bad_jaba(E_PARSER, "attempt to recall to a very old symbol");
}

bool Parser::cur_is_of(token_type type, bool load_next){
    bool tmp = cur_token -> get_type() == type;
    if((cur_token -> get_type() == ENDOF) && !tmp && load_next)
        throw bad_jaba(E_PARSER, "unexpected EOF");
    clog << "trying " << *cur_token << "\t-> for " << type << "\t" << endl;
//    Token tmp_tok = *cur_token;
    if(!tmp) return false;
//    prev_token = cur_token;
//    recall_available = true;
//    if(next_available){
//        next_available = false;
//        cur_token = next_token;
//    }else{
    if(load_next){
        token_buf.push_back(lexer -> get_token());
        cur_token++;
        cur_n++;
    }
//        stored_tokens++;
//        while(stored_tokens > 2){
//            token_buf.pop_front();
//            stored_tokens--;
//            if(prev_token == token_buf.begin())
//                recall_available = false;
//        }
//    }

    return true;
}

bool Parser::Element1(){
    /// function Identifier ( ParameterListOpt ) CompoundStatement
    return cur_is_of(FUNCTION) && cur_is_of(IDENTIFIER) && cur_is_of(OPEN_PAR) &&
            ParameterListOpt() && cur_is_of(CLOSE_PAR) && CompoundStatement();
}

bool Parser::ParameterList2(){
    /// Identifier , ParameterList
    return Ident() && cur_is_of(COMMA) && ParameterList();
}

bool Parser::CompoundStatement(){
    /// { Statements }
    return cur_is_of(OPEN_BRACE) && Statements() && cur_is_of(CLOSE_BRACE);
}

bool Parser::Statement1(){
    /// ;
    return cur_is_of(SEMICOLON);
}

bool Parser::Statement2(){
    /// if Condition Statement ElseNone
    return cur_is_of(IF) && Condition() && Statement() && ElseNone();
}

bool Parser::Statement3(){
    /// while Condition Statement
    return cur_is_of(WHILE) && Condition() && Statement();
}

bool Parser::Statement4(){
    /// ForParen ; ExpressionOpt ; ExpressionOpt ) Statement
    return ForParen() && cur_is_of(SEMICOLON) && ExpressionOpt() &&
        cur_is_of(SEMICOLON) && ExpressionOpt() && cur_is_of(CLOSE_PAR) && Statement();
}

bool Parser::Statement5(){
    /// break ;
    return cur_is_of(BREAK) && cur_is_of(SEMICOLON);
}

bool Parser::Statement6(){
    /// continue ;
    return cur_is_of(CONTINUE) && cur_is_of(SEMICOLON);
}

bool Parser::Statement7(){
    /// return ExpressionOpt ;
    return cur_is_of(RETURN) && ExpressionOpt() && cur_is_of(SEMICOLON);
}

bool Parser::Statement9(){
    /// VariablesOrExpression ;
    return VariablesOrExpression() && cur_is_of(SEMICOLON);
}

bool Parser::Condition(){
    /// ( Expression )
    return cur_is_of(OPEN_PAR) && Expression() && cur_is_of(CLOSE_PAR);
}

bool Parser::ElseNone1(){
    /// else Statement
    return cur_is_of(ELSE) && Statement();
}

bool Parser::ForParen(){
    /// for (VariablesOrExpression
    return cur_is_of(FOR) && cur_is_of(OPEN_PAR) && VariablesOrExpression();
}

bool Parser::VariablesOrExpression1(){
    /// var Variables
    return cur_is_of(VAR) && Variables();
}

bool Parser::Variables2(){
    /// Variable , Variables
    return Variable() && cur_is_of(COMMA) && Variables();
}

bool Parser::Variable2(){
    /// Identifier = AssignmentExpression
    return Ident() && cur_is_of(ASSIGN) && AssignmentExpression();
}

bool Parser::Expression2(){
    /// AssignmentExpression , Expression
    return AssignmentExpression() && cur_is_of(COMMA) && Expression();
}

bool Parser::AssignmentExpression1(){
    /// ConditionalExpression AssignmentOperator AssignmentExpression
    return ConditionalExpression() && AssignmentOperator() && AssignmentExpression();
}

bool Parser::ConditionalExpression2(){
    /// OrExpression ? AssignmentExpression : AssignmentExpression
    return OrExpression() && cur_is_of(QUESTION) && AssignmentExpression() &&
        cur_is_of(COLON) && AssignmentExpression();
}

bool Parser::OrExpression2(){
    /// AndExpression || OrExpression
    return AndExpression() && cur_is_of(OR) && OrExpression();
}

bool Parser::AndExpression2(){
    /// EqualityExpression && AndExpression
    return EqualityExpression() && cur_is_of(AND) && AndExpression();
}

bool Parser::EqualityExpression2(){
    /// RelationalExpression EqualityualityOperator EqualityExpression
    return RelationalExpression() && EqualityualityOperator() && EqualityExpression();
}

bool Parser::RelationalExpression2(){
    /// RelationalExpression RelationalOperator AdditiveExpression
    return AdditiveExpression() && RelationalOperator() && AdditiveExpression();
}

bool Parser::AdditiveExpression2(){
    /// MultiplicativeExpression + AdditiveExpression
    return MultiplicativeExpression() && cur_is_of(PLUS) && AdditiveExpression();
}

bool Parser::AdditiveExpression3(){
    /// MultiplicativeExpression - AdditiveExpression
    return MultiplicativeExpression() && cur_is_of(MINUS) && AdditiveExpression();
}

bool Parser::MultiplicativeExpression2(){
    /// UnaryExpression MultiplicativeOperator MultiplicativeExpression
    return UnaryExpression() && MultiplicativeOperator()/** * / % etc **/ && MultiplicativeExpression();
}

bool Parser::UnaryExpression2(){
    /// UnaryOperator UnaryExpression
    return UnaryOperator() && UnaryExpression();
}

bool Parser::UnaryExpression3(){
    /// - UnaryExpression
    return cur_is_of(MINUS) && UnaryExpression();
}

bool Parser::MemberExpression2(){
    /// PrimaryExpression . MemberExpression
    return PrimaryExpression() && cur_is_of(DOT) && MemberExpression();
}

bool Parser::MemberExpression3(){
    /// PrimaryExpression [ Expression ]
    return PrimaryExpression() && cur_is_of(OPEN_BRACKET) && Expression() && cur_is_of(CLOSE_BRACKET);
}

bool Parser::MemberExpression4(){
    /// PrimaryExpression ( ArgumentListOpt )
    return PrimaryExpression() && cur_is_of(OPEN_PAR) && ArgumentListOpt() && cur_is_of(CLOSE_PAR);
}

bool Parser::ArgumentList2(){
    /// AssignmentExpression , ArgumentList
    return AssignmentExpression() && cur_is_of(COMMA) && ArgumentList();
}

bool Parser::PrimaryExpression1(){
    ///  ( Expression )
    return cur_is_of(OPEN_PAR) && Expression() && cur_is_of(CLOSE_PAR);
}

bool Parser::PrimaryExpression3(){
    /// IntegerLiteral
    return cur_is_of(INT);
}

bool Parser::PrimaryExpression4(){
    /// FloatingPointLiteral
    return cur_is_of(REAL);
}

bool Parser::PrimaryExpression5(){
    /// StringLiteral
    return cur_is_of(STRING);
}

bool Parser::PrimaryExpression6(){
    /// false
    return cur_is_of(FALSE);
}

bool Parser::PrimaryExpression7(){
    /// true
    return cur_is_of(TRUE);
}

bool Parser::PrimaryExpression8(){
    /// null
    return cur_is_of(NIL);
}

bool Parser::Ident(){
    return cur_is_of(IDENTIFIER) || cur_is_of(RESPONCE) || cur_is_of(ENVIRONMENT);
}

bool Parser::UnaryOperator(){
    return cur_is_of(EXCLAMATION);
}

bool Parser::IncrementOperator(){
    return cur_is_of(INC) || cur_is_of(DEC);
}

bool Parser::MultiplicativeOperator(){
    return cur_is_of(TIMES) || cur_is_of(SLASH) || cur_is_of(PERCENT);
}

bool Parser::RelationalOperator(){
    return cur_is_of(GREATER) || cur_is_of(LESS) || cur_is_of(G_EQUAL) ||
            cur_is_of(L_EQUAL);
}

bool Parser::EqualityualityOperator(){
    return cur_is_of(EQUAL) || cur_is_of(N_EQUAL) || cur_is_of(IDENTITY) || cur_is_of(N_IDENTITY);
}

bool Parser::AssignmentOperator(){
    return cur_is_of(ASSIGN) || cur_is_of(PL_AS) || cur_is_of(MIN_AS) ||
            cur_is_of(TI_AS)|| cur_is_of(SL_AS);
}

void Parser::parse(){
    token_buf.push_front(lexer -> get_token());
    cur_token = token_buf.begin();
    cur_n = 1;
//    prev_token = cur_token;
//    next_available = false;
//    recall_available = false;
//    stored_tokens = 0;
    if(Program())
        clog << "nice work" << endl;
    else
        clog << "bad." << endl;
}
