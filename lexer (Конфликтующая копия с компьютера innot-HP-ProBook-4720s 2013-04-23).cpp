#include "lexer.h"

char *signs[] = {
    "", /// 0 is returned if nothing  during the search
    ".",
    ",",
    ";",
    ":",
    "(",
    ")",
    "{",
    "}",
    "+",
    "-",
    "*",
    "/",
    "%",
    ">",
    "<",
    "==",
    "!=",
    ">=",
    "<=",
    "&&",
    "||",
    "++",
    "--",
    "===",
    "!==",
    0
};

char *signs[]{
    ZERO,
    DOT,
    COMMA,
    SEMICOLON,
    COLON,
    OPEN_PAR,
    CLOSE_PAR,
    OPEN_BRACE,
    CLOSE_BRACE,
    PLUS,
    MINUS,
    TIMES,
    SLASH,
    PERCENT,
    GREATER,
    LESS,
    EQUAL,
    N_EQUAL,
    G_EQUAL,
    L_EQUAL,
    AND,
    OR,
    INC,
    DEC,
    IDENTITY,
    N_IDENTITY
}

ostream& Lexeme::operator<< (ostream &out, Lexeme what){
    out << what.type << '@' << what.position << endl;
    return out;
}

Identifier::Identifier(const Identifier& source){
    if(declared){
        delete name;
        delete value;
    }
    name = source.name;
    value = source.value;
    type = source.type;
    declared = source.declared;
}

Identifier::~Identifier(){
    if(declared){
        delete name;
        delete value;
    }
}

void Identifier::set_name(const string new_name){
    if(declared) throw bad_jaba("lexer", "name being redeclared");
    name = new_name;
    declared = true;
}

void Identifier::set_value(const string new_value){
    if(!declared) throw bad_jaba("lexer", "trying to assign something to a variable without a name");
    if(value)
        delete value;
    value = new_value;
}

void Identifier::set_type(lexeme_type new_type){
    if(!declared) throw bad_jaba("lexer", "assigning type to something undeclared");
    type = new_type;
}

Lexer::Lexer(const char *program){
    source = fopen(program, "r");
    cur_state = 0;
    clear_buffer();
    next();
}

Lexer::~Lexer(){
    //dtor
}

void Lexer::clear_buffer(){
    for(int i = 0; i < BUFSIZE; buffer[i++] = '\0');
    buffered_elements = 0;
}

void Lexer::push(){
    if(buffered_elements == BUFSIZE)
        throw bad_jaba("lexer", "buffer overflow");
    buffer[buffered_elements++] = c;
}

void Lexer::search(const char *needle, const char **haystack){
    for(int i = 0; haystack[i]; i++)
        if(needle == haystack[i])
            return i;
    return 0;
}

Lexeme Lexer::get_lexeme(){
    switch(cur_state){
        case INIT:
            if(c == ' ' || c == '\n' || c == '\r' || c == '\t')
                next();
            else if(c == EOF)
                return Lexeme(???);

    }
}
