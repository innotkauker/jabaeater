#include "rpn.h"

int RPN::priority_array[operators_number][2] = {
    {DOT, 7},
    {VAR, 6},
    {EXCLAMATION, 5},
    {TYPEOF, 5},
    {TIMES, 4},
    {SLASH, 4},
    {PERCENT, 4},
    {PLUS, 3},
    {MINUS, 3},
    {LESS, 2},
    {L_EQUAL, 2},
    {GREATER, 2},
    {G_EQUAL, 2},
    {EQUAL, 1},
    {N_EQUAL, 1},
    {IDENTITY, 1},
    {N_IDENTITY, 1},
    {ASSIGN, 0},
    {RETURN, -1}
};

int RPN::void_to_array(){
    to_array(Token(RPN_LINK, 0));
    return rpn_array.size() - 1;
}

void RPN::to_array(Token what, int where){
    if(where != -1)
        rpn_array.at(where) = what;
    else
        rpn_array.push_back(what);
}

void RPN::function_to_stack(Token object, Token method){
    method.set_type(FUNCTION);
    switch(object.get_type()){
        case RESPONCE:
            if(method.get_string_value() == string("write")){
                method.set_int_v(R_W);
            } else if(method.get_string_value() == string("endl")){
                method.set_int_v(R_E);
            } else
                throw bad_jaba(E_PARSER, "unknown method", method.get_line(), method.get_pos());
            break;
        case ENVIRONMENT:
            method.set_int_v(E_A);
            break;
        case IDENTIFIER:
        case STRING:
            if(method.get_string_value() == "length")
                method.set_int_v(S_L);
            else if(method.get_string_value() == "toInt")
                method.set_int_v(S_2I);
            else if(method.get_string_value() == "toReal")
                method.set_int_v(S_2R);
            else throw bad_jaba(E_PARSER, "unknown method", method.get_line(), method.get_pos());
            break;
        default:
            throw bad_jaba(E_PARSER, "unknown method", method.get_line(), method.get_pos());
    }
    to_stack(method);
}

void RPN::to_open_parenthesis(bool remove){
    while((rpn_stack.top().get_type() != OPEN_PAR) && !rpn_stack.empty()){
        to_array(rpn_stack.top());
        rpn_stack.pop();
    }
    if(rpn_stack.empty())
        throw bad_jaba(E_PARSER, "parentheses balance lost");
    if(remove){ /// remove '('
        rpn_stack.pop();
        if(!rpn_stack.empty() && (rpn_stack.top().get_type() == FUNCTION)){
            to_array(rpn_stack.top());
            rpn_stack.pop();
        }
    }
}

void RPN::put_op(Token what, bool left_associative){
    while(!rpn_stack.empty() && (left_associative ? compare(what, rpn_stack.top()) <= 0 : compare(what, rpn_stack.top()) < 0)){
        to_array(rpn_stack.top());
        rpn_stack.pop();
    }
    rpn_stack.push(what);
}

int RPN::compare(Token &a, Token &b){
    int a_priority = -10, b_priority = -10;
    for(int i = 0; i < operators_number; i++){
        if(a.get_type() == priority_array[i][0])
            a_priority = priority_array[i][1];
        if(b.get_type() == priority_array[i][0])
            b_priority = priority_array[i][1];
    }
    if(a_priority == -10)
        throw bad_jaba(E_GLOBAL, "internal error 0");
    if(a_priority < b_priority)
        return -1;
    if(a_priority == b_priority)
        return 0;
    return 1;
}

void RPN::out_array(){
    for(unsigned int i = 0; i < rpn_array.size(); i++)
        clog << i << ")\t" << rpn_array[i];
}

void RPN::stack_to_array(){
    while(!rpn_stack.empty()){
        to_array(rpn_stack.top());
        rpn_stack.pop();
    }
}
