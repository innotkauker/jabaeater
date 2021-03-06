#include "parser.h"

Token Parser::next(){
    return cur = (lexer -> get_token());
}

void Parser::P(){
    if(cur_is_of(ENDOF)){
        if(logging) clog << "P -> $" << endl;
    } else if(cur_is_of(FUNCTION)){
        if(logging) clog << "P -> function() CS $" << endl;
        next();
        if(!cur_is_of(OPEN_PAR))
            throw bad_jaba(E_PARSER, "'(' expected", cur.get_line(), cur.get_pos());
        next();
        if(!cur_is_of(CLOSE_PAR))
            throw bad_jaba(E_PARSER, "')' expected", cur.get_line(), cur.get_pos());
        next();
        CS();
        if(!cur_is_of(ENDOF))
            throw bad_jaba(E_PARSER, "EOF expected", cur.get_line(), cur.get_pos());
    } else
        throw bad_jaba(E_PARSER, "function expected", cur.get_line(), cur.get_pos());
}

void Parser::CS(int break_pos, int cont_pos){
    if(!cur_is_of(OPEN_BRACE))
        throw bad_jaba(E_PARSER, "'{' expected", cur.get_line(), cur.get_pos());
    if(logging) clog << "CS -> { Ss }" << endl;
    next();
    Ss(break_pos, cont_pos);
    if(!cur_is_of(CLOSE_BRACE))
        throw bad_jaba(E_PARSER, "'}' expected", cur.get_line(), cur.get_pos());
    next();
}

void Parser::S(int break_pos, int cont_pos){
    if(cur_is_of(SEMICOLON)){
        next();
        if(logging) clog << "S -> ;" << endl;
    } else if(cur_is_of(IF)){
        next();
        if(logging) clog << "S -> if C S EN" << endl;
        C();

        int ret_pos = rpn.void_to_array();
        rpn.to_array(Token(RPN_IFNOT_GOTO, 0));

        id_st.set_block();
        rpn.to_array(Token(OPEN_BRACE, string("{")));
        rpn.to_stack(Token(OPEN_PAR, string("(")));

        S(break_pos, cont_pos);

        rpn.to_open_parenthesis(true);
        id_st.drop();
        rpn.to_array(Token(CLOSE_BRACE, string("}")));

        EN(ret_pos);
    } else if(cur_is_of(WHILE)){
        next();
        if(logging) clog << "S -> while C S" << endl;
        int ret_pos = rpn.get_array_size();
        C();

        int exit_pos = rpn.void_to_array();
        rpn.to_array(Token(RPN_IFNOT_GOTO, 0));

        id_st.set_block();
        rpn.to_array(Token(OPEN_BRACE, string("{")));
        S(exit_pos, ret_pos);
        id_st.drop();
        rpn.to_array(Token(CLOSE_BRACE, string("}")));

        rpn.to_array(Token(INT, ret_pos));
        rpn.to_array(Token(RPN_GOTO, 0));

        rpn.to_array(Token(INT, rpn.get_array_size()), exit_pos);
    } else if(cur_is_of(FOR)){
        next();
        if(logging) clog << "S -> for (Ident V ; EO ; EO ) S" << endl;
        if(!cur_is_of(OPEN_PAR))
            throw bad_jaba(E_PARSER, "'(' expected", cur.get_line(), cur.get_pos());
        rpn.to_stack(cur);
        next();
        if(!Ident())
            throw bad_jaba(E_PARSER, "Identifier expected", cur.get_line(), cur.get_pos());
        rpn.to_array(cur);
        next();
        V();
        rpn.to_open_parenthesis(true);
        if(!cur_is_of(SEMICOLON))
            throw bad_jaba(E_PARSER, "';' expected", cur.get_line(), cur.get_pos());
        rpn.to_array(cur);
        next();
        int e2 = rpn.get_array_size();
        EO();

        int exit_pos = rpn.void_to_array();
        rpn.to_array(Token(RPN_IFNOT_GOTO, 0));
        int s_pos = rpn.void_to_array();
        rpn.to_array(Token(RPN_GOTO, 0));

        if(!cur_is_of(SEMICOLON))
            throw bad_jaba(E_PARSER, "';' expected", cur.get_line(), cur.get_pos());
        next();
        int e3 = rpn.get_array_size();
        EO();
        rpn.to_array(Token(SEMICOLON, string(";")));
        if(!cur_is_of(CLOSE_PAR))
            throw bad_jaba(E_PARSER, "')' expected", cur.get_line(), cur.get_pos());
        next();

        rpn.to_array(Token(INT, e2));
        rpn.to_array(Token(RPN_GOTO, 0));

        id_st.set_block();
        rpn.to_array(Token(INT, rpn.get_array_size()), s_pos);
        rpn.to_array(Token(OPEN_BRACE, string("{")));


        S(exit_pos, e3);

        rpn.to_array(Token(CLOSE_BRACE, string("}")));
        id_st.drop();

        rpn.to_array(Token(INT, e3));
        rpn.to_array(Token(RPN_GOTO, 0));

        rpn.to_array(Token(INT, rpn.get_array_size()), exit_pos);
    } else if(cur_is_of(BREAK)){
        if(break_pos == -1)
            throw bad_jaba(E_PARSER, "break unexpected", cur.get_line(), cur.get_pos());
        next();
        if(logging) clog << "S -> break;" << endl;
        if(!cur_is_of(SEMICOLON))
            throw bad_jaba(E_PARSER, "';' expected", cur.get_line(), cur.get_pos());
        next();
        rpn.to_array(Token(CLOSE_BRACE, string("}")));
//        rpn.to_array(Token(FALSE, false));
        rpn.to_array(Token(INT, break_pos));
        rpn.to_array(Token(RPN_GOTO, 0));
    } else if(cur_is_of(CONTINUE)){
        if(cont_pos == -1)
            throw bad_jaba(E_PARSER, "continue unexpected", cur.get_line(), cur.get_pos());
        next();
        if(logging) clog << "S -> continue;" << endl;
        if(!cur_is_of(SEMICOLON))
            throw bad_jaba(E_PARSER, "';' expected", cur.get_line(), cur.get_pos());
        next();
        rpn.to_array(Token(CLOSE_BRACE, string("}")));
        rpn.to_array(Token(INT, cont_pos));
        rpn.to_array(Token(RPN_GOTO, 0));
    } else if(cur_is_of(RETURN)){
        rpn.put_op(cur, false);
        next();
        if(logging) clog << "S -> UE E" << endl;

        rpn.to_stack(Token(OPEN_PAR, string("("), cur.get_line(), cur.get_pos()));
        UE();
        E();
        rpn.to_open_parenthesis(true);

        if(!cur_is_of(SEMICOLON))
            throw bad_jaba(E_PARSER, "';' expected", cur.get_line(), cur.get_pos());
        next();

    } else if(cur_is_of(OPEN_BRACE)){ /// checking
        if(logging) clog << "S -> CS" << endl;
        CS(break_pos, cont_pos);
    } else {
        if(logging) clog << "S -> VOE" << endl;
        VOE();
        if(!cur_is_of(SEMICOLON))
            throw bad_jaba(E_PARSER, "';' expected", cur.get_line(), cur.get_pos());
        rpn.to_array(cur);
        next();
    }

}

void Parser::Ss(int break_pos, int cont_pos){
    if(cur_is_of(CLOSE_BRACE)){ /// checking
        if(logging) clog << "Ss -> eps" << endl;
    } else {
        if(logging) clog << "Ss -> S Ss" << endl;
        rpn.to_stack(Token(OPEN_PAR, string("("), cur.get_line(), cur.get_pos()));
        S(break_pos, cont_pos);
        rpn.to_open_parenthesis(true);
        Ss(break_pos, cont_pos);
    }
}

void Parser::EN(int ret_pos){
    if(cur_is_of(ELSE)){
        next();
        if(logging) clog << "EN -> else S" << endl;
        int ret_pos2 = rpn.void_to_array();
        rpn.to_array(Token(RPN_GOTO, 0));
        rpn.to_array(Token(INT, rpn.get_array_size()), ret_pos);
        id_st.set_block();
        rpn.to_array(Token(OPEN_BRACE, string("{"), cur.get_line(), cur.get_pos()));
        rpn.to_stack(Token(OPEN_PAR, string("("), cur.get_line(), cur.get_pos()));
        S();
        rpn.to_open_parenthesis(true);
        id_st.drop();
        rpn.to_array(Token(CLOSE_BRACE, string("}"), cur.get_line(), cur.get_pos()));
        rpn.to_array(Token(INT, rpn.get_array_size()), ret_pos2);
    } else {
        if(logging) clog << "EN -> eps" << endl;
        rpn.to_array(Token(INT, rpn.get_array_size()), ret_pos);
    }
}

void Parser::C(){
    if(!cur_is_of(OPEN_PAR))
        throw bad_jaba(E_PARSER, "'(' expected", cur.get_line(), cur.get_pos());
    rpn.to_stack(cur);
    next();
    if(logging) clog << "C -> (UE E)" << endl;
    UE();
    E();
    if(!cur_is_of(CLOSE_PAR))
        throw bad_jaba(E_PARSER, "')' expected", cur.get_line(), cur.get_pos());
    rpn.to_open_parenthesis();
    next();
}

void Parser::VOE(){
    if(cur_is_of(VAR)){
        rpn.to_stack(Token(OPEN_PAR, "(", cur.get_line(), cur.get_pos()));

        rpn.put_op(cur);
        next();
        if(logging) clog << "VOE -> var Ident V {, Ident V}" << endl;
        if(!Ident(true))
            throw bad_jaba(E_PARSER, "Identifier expected", cur.get_line(), cur.get_pos());
        rpn.to_array(cur);
        next();
        V();
        rpn.to_open_parenthesis(true);
        //rpn.to_array(Token(SEMICOLON, string(";")));

        while(cur_is_of(COMMA)){
            next();

            rpn.to_stack(Token(OPEN_PAR, "(", cur.get_line(), cur.get_pos()));

            rpn.put_op(Token(VAR, string("var")));
            if(!Ident(true))
                throw bad_jaba(E_PARSER, "Identifier expected", cur.get_line(), cur.get_pos());
            rpn.to_array(cur);
            next();
            V();
            rpn.to_open_parenthesis(true);
            //rpn.to_array(Token(SEMICOLON, string(";")));
        }
    } else {
        if(logging) clog << "VOE -> UE E" << endl;

        rpn.to_stack(Token(OPEN_PAR, "(", cur.get_line(), cur.get_pos()));

        UE();
        E();

        rpn.to_open_parenthesis(true);

    }
}

void Parser::V(){
    if(cur_is_of(ASSIGN)){
        rpn.put_op(cur, false);
        next();
        if(logging) clog << "V -> = UE E" << endl;
        UE();
        E();
    } else {
        if(logging) clog << "V -> eps" << endl;
    }
}

void Parser::EO(){
    if(cur_is_of(SEMICOLON)){ /// checking
        if(logging) clog << "EO -> eps" << endl;
    } else {
        if(logging) clog << "EO -> UE E" << endl;
        rpn.to_stack(Token(OPEN_PAR, "(", cur.get_line(), cur.get_pos()));
        UE();
        E();
        rpn.to_open_parenthesis(true);
    }
}

void Parser::E(){
    if(any_operator()){
        rpn.put_op(cur, false);
        next();
        if(logging) clog << "V -> any_op UE E" << endl;
        UE();
        E();
    } else {
        if(logging) clog << "E -> eps" << endl;
    }
}

void Parser::UE(){
    if(cur_is_of(EXCLAMATION)){
        rpn.put_op(cur, false);
        next();
        if(logging) clog << "UE -> ! UE" << endl;
        UE();
    } else if(cur_is_of(MINUS)){
        rpn.to_array(Token(INT, 0, cur.get_line(), cur.get_pos()));
        rpn.put_op(cur);
        next();
        if(logging) clog << "UE -> - UE" << endl;
        UE();
    } else {
        if(logging) clog << "UE -> PE MeE" << endl;
        object = cur;
        PE();
        MeE();
    }
}

void Parser::MeE(bool sub_exp){
    if(cur_is_of(DOT)){
        rpn.to_array(cur);
        next();
        if(logging) clog << "MeE -> . MeE" << endl;
        MeE(true);
    } else if(cur_is_of(IDENTIFIER) || cur_is_of(RESPONCE) || cur_is_of(ENVIRONMENT)){
        if(!sub_exp)
            throw bad_jaba(E_PARSER, "unexpected identifier", cur.get_line(), cur.get_pos());
        rpn.function_to_stack(object, cur);
        next();
        if(logging) clog << "MeE -> Ident(ALO)" << endl;
        if(!cur_is_of(OPEN_PAR))
            throw bad_jaba(E_PARSER, "'(' expected", cur.get_line(), cur.get_pos());
        rpn.to_stack(cur);
        next();
        ALO();
        if(!cur_is_of(CLOSE_PAR))
            throw bad_jaba(E_PARSER, "')' expected", cur.get_line(), cur.get_pos());
        rpn.to_open_parenthesis(true);
        next();
    }  else {
        if(logging) clog << "MeE -> eps" << endl;
    }
}

void Parser::PE(){
    switch(cur.get_type()){
        case OPEN_PAR:
            rpn.to_stack(cur);
            next();
            if(logging) clog << "PE -> (UE E)" << endl;
            E();
            if(!cur_is_of(CLOSE_PAR))
                throw bad_jaba(E_PARSER, "')' expected", cur.get_line(), cur.get_pos());
            rpn.to_open_parenthesis();
            /// object = ???
            next();
            break;
        case IDENTIFIER:
            if(!id_st.get(cur.get_string_value()))
                throw bad_jaba(E_IDENTIFIER, "Identifier not declared", cur.get_line(), cur.get_pos());
        case RESPONCE:
        case ENVIRONMENT:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> Ident" << endl;
            break;
        case INT:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> int" << endl;
            break;
        case REAL:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> real" << endl;
            break;
        case STRING:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> string" << endl;
            break;
        case FALSE:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> false" << endl;
            break;
        case TRUE:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> true" << endl;
            break;
        case NIL:
            rpn.to_array(cur);
            next();
            if(logging) clog << "PE -> null" << endl;
            break;
        default:
            throw bad_jaba(E_PARSER, "something expected", cur.get_line(), cur.get_pos());
    }
}

void Parser::ALO(){
    if(cur_is_of(CLOSE_PAR)){ /// checking
        if(logging) clog << "ALO -> eps" << endl;
    } else {
        if(logging) clog << "ALO -> EO {,EO}" << endl;
        EO();
        while(cur_is_of(COMMA)){
            rpn.to_open_parenthesis(false);
            next();
            EO();
        }
        rpn.to_open_parenthesis(false);
    }
}

bool Parser::cur_is_of(token_type type){
    return ((cur.get_type()) == type);
}

bool Parser::Ident(bool var){
    if(!(cur_is_of(IDENTIFIER) || cur_is_of(RESPONCE) || cur_is_of(ENVIRONMENT)))
        return false;
    if((var && id_st.defined_in_cur_block(cur.get_string_value())) || cur_is_of(RESPONCE) || cur_is_of(ENVIRONMENT))
        throw bad_jaba(E_IDENTIFIER, "Identifier redeclared", cur.get_line(), cur.get_pos());
    if(var)
        id_st.push(cur.get_string_value());
    else if(!id_st.get(cur.get_string_value()))
        throw bad_jaba(E_IDENTIFIER, "Identifier not declared", cur.get_line(), cur.get_pos());
    return true;
}

bool Parser::any_operator(){
    return cur_is_of(ASSIGN) || cur_is_of(GREATER) || cur_is_of(LESS) || cur_is_of(G_EQUAL) ||
            cur_is_of(L_EQUAL) || cur_is_of(EQUAL) || cur_is_of(N_EQUAL) ||
             cur_is_of(IDENTITY) || cur_is_of(N_IDENTITY) || cur_is_of(TIMES) ||
              cur_is_of(SLASH) || cur_is_of(PERCENT) || cur_is_of(PLUS) || cur_is_of(MINUS);
}

void Parser::parse(){
    next();
    P();
}
