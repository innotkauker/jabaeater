#ifndef BAD_JABA_H
#define BAD_JABA_H

#include <exception>
#include <string>
#include <iostream>
#include "filesource.h"

using std::ostream;
using std::endl;
using std::string;

enum e_types{
    E_GLOBAL,
    E_LEXER,
    E_IDENTIFIER,
    E_PARSER,
    E_RUNTIME
};

/// We throw() objects of this class
class bad_jaba: public std::exception{
    e_types type;
    string description;
    int line_n;
    int position; /// in line
public:
    bad_jaba(e_types e_type, const string &e_desc, int line = -1, int pos = -1);
    friend ostream& operator<<(ostream &out, const bad_jaba &what);
    virtual ~bad_jaba() throw() {}
};

#endif // BAD_JABA_H
