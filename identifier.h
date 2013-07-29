#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <stack>
#include <iostream>
#include "local_types.h"
#include "bad_jaba.h"
#include "rpn.h"
#include "lexer.h"

using std::stack;
using std::clog;
using std::endl;

enum identifier_type{
    ID_STRING,
    ID_INT,
    ID_BOOL,
    ID_REAL,
    ID_UNDEFINED
};

class Identifier{
    string name;
    string v_string;
    int v_int;
    double v_double;
    bool v_bool;
    identifier_type type;
public:
    Identifier(): v_string("undefined") {}
    Identifier(const Identifier &source);
    Identifier(const string iname):name(iname), v_string("undefined"), type(ID_UNDEFINED){}
//    Identifier(const Token &source);
    ~Identifier(){}
    string &get_name(){return name;}
    string &get_string(){return v_string;}
    int &get_int(){return v_int;}
    double &get_real(){return v_double;}
    bool &get_bool(){return v_bool;}
    identifier_type &get_type(){return type;}
//    void set_name(const string new_name);
//    void set_value(const string new_name);
//    void set_type(token_type new_type);
};

class IdentifierStack{
    Identifier *data;
    Identifier *first_free;
    int size;
    stack<int> block_indentions;
    bool logging;
public:
    IdentifierStack(int depth = 256, bool log = false);
    ~IdentifierStack(){delete[] data;}
    void push(Identifier what);
    Identifier *get(string name);
            /// returns a ptr to the first Id from the top or 0 if not found
    void drop(); /// drops current block
    void set_block();
    bool defined_in_cur_block(string name);
};

#endif // IDENTIFIER_H
