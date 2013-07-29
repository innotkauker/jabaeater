#ifndef EXECUTIONER_H
#define EXECUTIONER_H

#include <stack>
#include <sstream>
#include "rpn.h"
#include "lexer.h"
#include "bad_jaba.h"
#include "identifier.h"

class Executioner{
    ostream &output;
    RPN *rpn;
    Token cur;
    stack<Token> exec_stack;
    IdentifierStack id_st;
    Token &convert(Token &what, token_type to);
    token_type get_type(Token &what);
    void plus();
    void oth_binary(token_type type);
    void relational(token_type type);
    void function_t();
    void assignment();
public:
    Executioner(RPN *rpn_array, ostream &out = cout): output(out), rpn(rpn_array), id_st(1024, false){}
    int execute();
};

#endif // EXECUTIONER_H
