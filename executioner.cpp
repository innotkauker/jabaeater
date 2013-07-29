#include "executioner.h"

Token& Executioner::convert(Token &what, token_type to){
    if(what.get_type() == IDENTIFIER){
        Identifier *tmp = id_st.get(what.get_string_value());
        switch(tmp -> get_type()){
            case ID_STRING:
                what = Token(STRING, tmp -> get_string());
                break;
            case ID_INT:
                what = Token(INT, tmp -> get_int());
                break;
            case ID_REAL:
                what = Token(REAL, tmp -> get_real());
                break;
            case ID_BOOL:
                what = Token(BOOL, tmp -> get_bool());
                break;
            case ID_UNDEFINED:
                throw bad_jaba(E_RUNTIME, string("Identifier ") + tmp -> get_name() + string(" undefined"));
        }
    }
    stringstream tmp;
    string str;
    switch(what.get_type()){
        case STRING:
            switch(to){
                case INT:
                    what = Token(INT, atoi(what.get_string_value().c_str()));
                    break;
                case REAL:
                    what = Token(REAL, strtod(what.get_string_value().c_str(), 0));
                    break;
                case BOOL:
                    what = Token(BOOL, what.get_string_value().size() ? true : false);
                    break;
                default:
                    ;
            }
            break;
        case INT:
            switch(to){
                case STRING:
                    tmp << what.get_int_value();
                    tmp >> str;
                    what = Token(STRING, str);
                    break;
                case REAL:
                    what = Token(REAL, double(what.get_int_value()));
                    break;
                case BOOL:
                    what = Token(BOOL, what.get_int_value() ? true : false);
                    break;
                default:
                    ;
            }
            break;
        case REAL:
            switch(to){
                case STRING:
                    tmp << what.get_real_value();
                    tmp >> str;
                    what = Token(STRING, str);
                    break;
                case INT:
                    what = Token(REAL, int(what.get_real_value()));
                    break;
                case BOOL:
                    what = Token(BOOL, (int)what.get_real_value() ? true : false);
                    break;
                default:
                    ;
            }
            break;
        case BOOL:
            switch(to){
                case STRING:
                    what = Token(STRING, what.get_bool_value() ? string("true") : string("false"));
                    break;
                case REAL:
                    what = Token(REAL, what.get_bool_value() ? 1.0 : 0.0);
                    break;
                case INT:
                    what = Token(BOOL, what.get_bool_value() ? 1 : 0);
                    break;
                default:
                    ;
            }
            break;
        default:
            throw bad_jaba(E_RUNTIME, string("internal error"));
    }
    return what;
}

token_type Executioner::get_type(Token &what){
    Identifier *tmp;
    switch(what.get_type()){
        case IDENTIFIER:
            tmp = id_st.get(what.get_string_value());
            switch(tmp -> get_type()){
                case ID_STRING: return STRING;
                case ID_INT: return INT;
                case ID_REAL: return REAL;
                case ID_BOOL: return BOOL;
                case ID_UNDEFINED:
                    throw bad_jaba(E_RUNTIME, string("Identifier") + tmp -> get_name() + string("undefined"), what.get_line(), what.get_pos());
            }
        case TRUE:
        case FALSE:
            return BOOL;
        default:
            return what.get_type();
    }
}

void Executioner::plus(){
    Token op2 = exec_stack.top();
    exec_stack.pop();
    Token op1 = exec_stack.top();
    exec_stack.pop();
    if(get_type(op1) == STRING || get_type(op2) == STRING)
        exec_stack.push(Token(STRING, string(convert(op1, STRING).get_string_value()
                            + convert(op2, STRING).get_string_value())));
    else if(get_type(op1) == REAL || get_type(op2) == REAL)
        exec_stack.push(Token(REAL, convert(op1, REAL).get_real_value()
                              + convert(op2, REAL).get_real_value()));
    else
        exec_stack.push(Token(INT, convert(op1, INT).get_int_value()
                              + convert(op2, INT).get_int_value()));
}

void Executioner::oth_binary(token_type type){
    Token op2 = exec_stack.top();
    exec_stack.pop();
    Token op1 = exec_stack.top();
    exec_stack.pop();
    if(get_type(op1) == REAL || get_type(op2) == REAL){
        switch(type){
            case MINUS:
                exec_stack.push(Token(REAL, convert(op1, REAL).get_real_value()
                              - convert(op2, REAL).get_real_value()));
                break;
            case TIMES:
                exec_stack.push(Token(REAL, convert(op1, REAL).get_real_value()
                              * convert(op2, REAL).get_real_value()));
                break;
            case SLASH:
                if(!convert(op2, REAL).get_real_value())
                    throw bad_jaba(E_RUNTIME, string("division by zero"));
                exec_stack.push(Token(REAL, convert(op1, REAL).get_real_value()
                              / op2.get_real_value()));
                break;
            case PERCENT:
                exec_stack.push(Token(INT, convert(op1, INT).get_int_value()
                              % convert(op2, INT).get_int_value()));
                break;
            default:
                throw bad_jaba(E_GLOBAL, string("internal error"));
        }
    } else {
        switch(type){
            case MINUS:
                exec_stack.push(Token(INT, convert(op1, INT).get_int_value()
                              - convert(op2, INT).get_int_value()));
                break;
            case TIMES:
                exec_stack.push(Token(INT, convert(op1, INT).get_int_value()
                              * convert(op2, INT).get_int_value()));
                break;
            case SLASH:
                if(!convert(op2, REAL).get_real_value())
                    throw bad_jaba(E_RUNTIME, string("division by zero"));
                exec_stack.push(Token(REAL, convert(op1, REAL).get_real_value()
                              / convert(op2, REAL).get_real_value()));
                break;
            case PERCENT:
                exec_stack.push(Token(INT, convert(op1, INT).get_int_value()
                              % convert(op2, INT).get_int_value()));
                break;
            default:
                throw bad_jaba(E_GLOBAL, string("Internal error"));
        }
    }
}

void Executioner::relational(token_type type){
    Token op2 = exec_stack.top();
    exec_stack.pop();
    Token op1 = exec_stack.top();
    exec_stack.pop();
    if(get_type(op1) == STRING && get_type(op2) == STRING)
        switch(type){
            case EQUAL:
                exec_stack.push(Token(BOOL, op1.get_string_value() == op2.get_string_value()));
                break;
            case N_EQUAL:
                exec_stack.push(Token(BOOL, op1.get_string_value() != op2.get_string_value()));
                break;
            case LESS:
                exec_stack.push(Token(BOOL, op1.get_string_value() < op2.get_string_value()));
                break;
            case GREATER:
                exec_stack.push(Token(BOOL, op1.get_string_value() > op2.get_string_value()));
                break;
            case L_EQUAL:
                exec_stack.push(Token(BOOL, op1.get_string_value() <= op2.get_string_value()));
                break;
            case G_EQUAL:
                exec_stack.push(Token(BOOL, op1.get_string_value() >= op2.get_string_value()));
                break;
            default:
                ;
        }
    else {
        convert(op1, REAL);
        convert(op2, REAL);
        switch(type){
            case EQUAL:
                exec_stack.push(Token(BOOL, op1.get_real_value() == op2.get_real_value()));
                break;
            case N_EQUAL:
                exec_stack.push(Token(BOOL, op1.get_real_value() != op2.get_real_value()));
                break;
            case LESS:
                exec_stack.push(Token(BOOL, op1.get_real_value() < op2.get_real_value()));
                break;
            case GREATER:
                exec_stack.push(Token(BOOL, op1.get_real_value() > op2.get_real_value()));
                break;
            case L_EQUAL:
                exec_stack.push(Token(BOOL, op1.get_real_value() <= op2.get_real_value()));
                break;
            case G_EQUAL:
                exec_stack.push(Token(BOOL, op1.get_real_value() >= op2.get_real_value()));
                break;
            default:
                ;
        }
    }
}

void Executioner::function_t(){
    stack<Token> f_param_stack;
    char *env_str;
    int param_num = 0;
    Token op1, op2;
    while(!exec_stack.empty() && (exec_stack.top().get_type() != DOT)){
        f_param_stack.push(exec_stack.top());
        exec_stack.pop();
        param_num++;
    }
    if(exec_stack.empty())
        throw bad_jaba(E_PARSER, string("only method functions supported"), cur.get_line(), cur.get_pos());
    exec_stack.pop();
    op1 = exec_stack.top(); /// object
    exec_stack.pop();
    switch(get_type(op1)){
        case RESPONCE:
            if(cur.get_int_value() == R_W) /// write
                for(int i = 0; i < param_num; i++){
                    Token tmp = f_param_stack.top();
                    if(tmp.get_type() == IDENTIFIER)
                        convert(tmp, tmp.get_type());
                    tmp.print(output);
                    f_param_stack.pop();
                    exec_stack.push(Token(INT, 0));
                }
            else {/// endl
                if(param_num)
                    throw bad_jaba(E_PARSER, string("Responce.endl() takes only one parameter"), cur.get_line(), cur.get_pos());
                output << endl;
            }
            break;
        case ENVIRONMENT:
            if(param_num)
                throw bad_jaba(E_PARSER, string("Environment.") + cur.get_string_value() + string("() takes only one parameter"), cur.get_line(), cur.get_pos());
            env_str = getenv(cur.get_string_value().c_str());
            exec_stack.push(Token(STRING, string(env_str ? env_str : "")));
            break;
        case STRING:
            switch(cur.get_int_value()){
                case S_L:
                    if(param_num)
                        throw bad_jaba(E_PARSER, string("string.length() takes no parameters"), cur.get_line(), cur.get_pos());
                    exec_stack.push(Token(INT, (int)op1.get_string_value().length()));
                    break;
                case S_2I:
                    if(param_num)
                        throw bad_jaba(E_PARSER, string("string.toInt() takes no parameters"), cur.get_line(), cur.get_pos());
                    exec_stack.push(Token(INT, (int)strtol(op1.get_string_value().c_str(), 0, 10)));
                    break;
                case S_2R:
                    if(param_num)
                        throw bad_jaba(E_PARSER, string("string.toReal() takes no parameters"), cur.get_line(), cur.get_pos());
                    exec_stack.push(Token(REAL, strtod(op1.get_string_value().c_str(), 0)));
                    break;
                default:
                    ;
            }
            break;
        case IDENTIFIER:
            switch(get_type(op1)){
                case S_L:
                    if(param_num)
                        throw bad_jaba(E_PARSER, string("string.length() takes no parameters"), cur.get_line(), cur.get_pos());
                    exec_stack.push(Token(INT, (int)op1.get_string_value().length()));
                    break;
                case S_2I:
                    if(param_num)
                        throw bad_jaba(E_PARSER, string("string.toInt() takes no parameters"), cur.get_line(), cur.get_pos());
                    exec_stack.push(Token(INT, (int)strtol(op1.get_string_value().c_str(), 0, 10)));
                    break;
                case S_2R:
                    if(param_num)
                        throw bad_jaba(E_PARSER, string("string.toReal() takes no parameters"), cur.get_line(), cur.get_pos());
                    exec_stack.push(Token(REAL, strtod(op1.get_string_value().c_str(), 0)));
                    break;
                default:
                    ;
            }
            break;
        default:
            ;
    }
}

void Executioner::assignment(){
    Token op1, op2;
    Identifier *tmp;
    Identifier *tmp2;
    op2 = exec_stack.top();
    exec_stack.pop();
    op1 = exec_stack.top();
    exec_stack.pop();
    if(op1.get_type() != IDENTIFIER)
        throw bad_jaba(E_PARSER, string("lvalue required as the first operand of assignment"), cur.get_line(), cur.get_pos());
    tmp = id_st.get(op1.get_string_value());
    exec_stack.push(op1);
    switch(op2.get_type()){
        case IDENTIFIER:
            tmp2 = id_st.get(op2.get_string_value());
            tmp -> get_type() = tmp2 -> get_type();
            switch(tmp2 -> get_type()){
                case ID_STRING:
                    tmp -> get_string() = tmp2 -> get_string();
                    break;
                case ID_INT:
                    tmp -> get_int() = tmp2 -> get_int();
                    break;
                case ID_BOOL:
                    tmp -> get_bool() = tmp2 -> get_bool();
                    break;
                case ID_REAL:
                    tmp -> get_real() = tmp2 -> get_real();
                    break;
                default:
                    ;
            }
            break;
        case STRING:
            tmp -> get_string() = op2.get_string_value();
            tmp -> get_type() = ID_STRING;
            break;
        case INT:
            tmp -> get_int() = op2.get_int_value();
            tmp -> get_type() = ID_INT;
            break;
        case BOOL:
            tmp -> get_bool() = op2.get_bool_value();
            tmp -> get_type() = ID_BOOL;
            break;
        case REAL:
            tmp -> get_real() = op2.get_real_value();
            tmp -> get_type() = ID_REAL;
            break;
        case UNDEFINED:
            tmp -> get_type() = ID_UNDEFINED;
            break;
        default:
            ;
    }
}

int Executioner::execute(){
    Token op1, op2;
    for(int i = 0; i < rpn -> get_array_size(); i++){
        cur = rpn -> operator[](i);
        switch(cur.get_type()){
            case TRUE:
            case FALSE:
            case INT:
            case REAL:
            case STRING:
            case IDENTIFIER:
            case RPN_LINK:
            case DOT:
            case RESPONCE:
            case ENVIRONMENT:
                exec_stack.push(cur);
                break;
            case PLUS:
                plus();
                break;
            case MINUS:
            case TIMES:
            case SLASH:
            case PERCENT:
                oth_binary(cur.get_type());
                break;
            case GREATER:
            case LESS:
            case G_EQUAL:
            case L_EQUAL:
            case N_EQUAL:
            case EQUAL:
                relational(cur.get_type());
                break;
            case EXCLAMATION:
                op1 = exec_stack.top();
                exec_stack.pop();
                exec_stack.push(Token(BOOL, !convert(op1, BOOL).get_bool_value()));
                break;
            case VAR:
                op1 = exec_stack.top();
                exec_stack.pop();
                id_st.push(op1.get_string_value());
                exec_stack.push(op1);
                break;
            case ASSIGN:
                assignment();
                break;
            case SEMICOLON:
                exec_stack.pop();
                break;
            case OPEN_BRACE:
                id_st.set_block();
                break;
            case CLOSE_BRACE:
                id_st.drop();
                break;
            case RETURN:
                op1 = exec_stack.top();
                exec_stack.pop();
                return convert(op1, INT).get_int_value();
            case RPN_GOTO:
                op1 = exec_stack.top();
                exec_stack.pop();
                i = op1.get_int_value() - 1;
                break;
            case RPN_IFNOT_GOTO:
                op1 = exec_stack.top();
                exec_stack.pop();
                op2 = exec_stack.top();
                exec_stack.pop();
                if(!convert(op2, BOOL).get_bool_value())
                    i = op1.get_int_value() - 1;
                break;
            case FUNCTION:
                function_t();
                break;
            default:
                ;
        }
    }
    return 0;
}

