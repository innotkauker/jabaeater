#include "bad_jaba.h"

bad_jaba::bad_jaba(e_types e_type, const string &e_desc, int line, int pos){
    type = e_type;
    description = e_desc;
    line_n = line;
    position = pos;
}

ostream& operator<<(ostream &out, const bad_jaba &what){
    switch(what.type){
        case E_IDENTIFIER:
            out << "Identifier: ";
            break;
        case E_GLOBAL:
            out << "Global: ";
            break;
        case E_LEXER:
            out << "Lexer: ";
            break;
        case E_PARSER:
            out << "Parser: ";
            break;
        case E_RUNTIME:
            out << "Runtime error: ";
            break;
    }
    out << what.description;
    if(what.line_n != -1){
        out << " @ " << what.line_n;
        if(what.position != -1)
            out << ":" << what.position << endl;
    }
    return out;
}

