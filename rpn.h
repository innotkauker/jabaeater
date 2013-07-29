#ifndef RPN_H
#define RPN_H

#include <vector>
#include <stack>
#include "lexer.h"

using std::vector;
using std::stack;

const int operators_number = 19;

enum method_ids{
    R_W = 0, /// responce.write()
    R_E, /// responce.endl()
    E_A, /// environment.x()
    S_2I, /// string.toInt()
    S_2R, /// string.toInt()
    S_L /// string.length()
};

class RPN{
    static int priority_array[][2];
    vector<Token> rpn_array;
    stack<Token> rpn_stack;
public:
    int void_to_array();
    void to_array(Token what, int where = -1); /// puts what to RPN array
    void to_open_parenthesis(bool remove = true); /// pops stack contents
                                                  /// to array up to '('
    void put_op(Token what, bool left_associative = true); /// "what" is some operation
    int compare(Token &a, Token &b);
    void to_stack(Token what){rpn_stack.push(what);}
    void function_to_stack(Token object, Token method);
    void stack_to_array();
    void out_array();
    int get_array_size(){return rpn_array.size();}
    Token &operator[](int i){return rpn_array[i];}
};

#endif // RPN_H
