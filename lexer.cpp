#include "lexer.h"

int Lexer::DFA[][11] = { /// Our automaton table
///              _INIT,      _LEXEME,        _INT,       _STRING1,   _DIV,           _REAL3,        _TRIPLE,        _DOUBLE,        _SINGLE,        _COMMENT,   _REAL0
/**SPACE*/   {   _INIT,      _OUT_LEX,       _OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**NEWLN*/   {   _INIT,      _OUT_LEX,       _OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _INIT,      _BAD_REAL0_ERR },
/**EOF*/     {   _OUT_DELIMITER,_OUT_LEX,    _OUT_INT,   _UNTERM_STR_ERR,_OUT_DELIMITER,_OUT_REAL0, _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**CHAR*/    {   _LEXEME,    _LEXEME,        _OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**DIGIT*/   {   _INT,       _LEXEME,        _INT,       _STRING1,   _OUT_DELIMITER, _REAL3,        _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _REAL3         },
/**APOS1*/   {   _STRING1,   _OUT_LEX,       _OUT_INT,   _OUT_STRING,_OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**SLASH*/   {   _DIV,       _OUT_LEX,       _OUT_INT,   _STRING1,   _COMMENT,       _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**EQU*/     {   _TRIPLE,    _OUT_LEX,       _OUT_INT,   _STRING1,   _OUT_DELIMITER,        _OUT_REAL0,    _DOUBLE,        _SINGLE,        _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**DOUBLE*/  {   _DOUBLE,    _OUT_LEX,       _OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**DOT*/     {   _SINGLE,    _OUT_LEX,       _REAL0,     _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**NOT*/     {   _TRIPLE,    _OUT_LEX,       _OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**OTHER*/   {   _SINGLE,    _OUT_LEX,       _OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR },
/**UNRECOGNIZED*/{_UNKNOWN_CHAR_ERR,_OUT_LEX,_OUT_INT,   _STRING1,   _OUT_DELIMITER, _OUT_REAL0,    _OUT_DELIMITER, _OUT_DELIMITER, _OUT_DELIMITER, _COMMENT,   _BAD_REAL0_ERR }
};

string Lexer::signs[] = {
    "t", /// 0 is returned if nothing is found during the search
    ".",
    ",",
    ";",
    ":",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]",
    "+",
    "-",
    "*",
    "/",
    "!",
    "%",
    ">",
    "<",
    "=",
    "==",
    "!=",
    ">=",
    "<=",
    "===",
    "!==",
    "\0"
};

token_type Lexer::sig_names[] = {
    ZERO, DOT, COMMA, SEMICOLON,
    COLON, OPEN_PAR, CLOSE_PAR, OPEN_BRACE,
    CLOSE_BRACE, OPEN_BRACKET, CLOSE_BRACKET, PLUS,
    MINUS, TIMES, SLASH, EXCLAMATION, PERCENT, GREATER, LESS,
    ASSIGN, EQUAL, N_EQUAL, G_EQUAL,
    L_EQUAL, IDENTITY,
    N_IDENTITY, ZERO
};

string Lexer::reserved_words[] = {
    "*",
    "var",
    "true",
    "false",
    "Responce",
    "Environment",
    "null",
    "undefined",
    "typeof",
    "function",
    "if",
    "while",
    "for",
    "else",
    "break",
    "continue",
    "return",
    "\0"
};

token_type Lexer::res_w_names[] = {
    ZERO, VAR, TRUE, FALSE, RESPONCE, ENVIRONMENT, NIL,
    UNDEFINED, TYPEOF, FUNCTION, IF,
    WHILE, FOR, ELSE, BREAK, CONTINUE, RETURN, ZERO
};

ostream& operator<< (ostream &where, Token what){
    where << what.type << '\t';
    switch(what.type){
        case INT:
            where << what.v_int;
            break;
        case REAL:
            where << what.v_double;
            break;
        case TRUE:
            where << "true";
            break;
        case FALSE:
            where << "false";
            break;
        case RPN_GOTO:
            where << "goto";
            break;
        case RPN_IFNOT_GOTO:
            where << "if false goto";
            break;
        case RPN_LINK:
            where << what.v_int;
            break;
        default:
            where << what.v_string;
    }
    where << endl;
    return where;
}

void Token::print(ostream &where){
    switch(type){
        case INT:
            where << v_int;
            break;
        case REAL:
            where << v_double;
            break;
        case TRUE:
            where << "true";
            break;
        case FALSE:
            where << "false";
            break;
        default:
            where << v_string;
    }
}

Lexer::Lexer(FileSource &file){
    source = file;
    cur_state = _INIT;
    clear_buffer();
    pure = true;
    token_is_ready = false;
}

int Lexer::search(const string needle, const string *haystack){
    for(int i = 0; haystack[i][0]; i++)
        if(needle == haystack[i])
            return i;
    return 0;
}

symbol Lexer::type(char smb){
    switch(smb){
        case ' ': return __SPACE;
        case '\t': return __SPACE;
        case '\n': return __NEWLN;
        case EOF: return __EOF;
        case '"': return __APOS1;
//        case '\'': return __APOS2;
        case '/': return __SLASH;
        case '=': return __EQU;
        case '>': return __DOUBLE;
        case '<': return __DOUBLE;
//        case '*': return __DOUBLE;
//        case '+': return __PLUS;
//        case '-': return __MINUS;
        case '.': return __DOT;
//        case 'E': return __Ee;
//        case 'e': return __Ee;
        case '!': return __NOT;
        default:
            if(isalpha(smb) || smb == '_')
                return __CHAR;
            if(isdigit(smb))
                return __DIGIT;
            string other(":;,?+-*(){}[]");
            if(string::npos != other.find_first_of(smb))
                return __OTHER;
            return __UNRECOGNIZED;
    }
}

bool Lexer::use_next_smb(char smb){
    int i;
    if(pure){
        pure = false;
        token_is_ready = false;
    }
    switch(cur_state){
        case _OUT_LEX:
            source.unget_c(c);
            token_is_ready = true;
            if((i = search(buffer, reserved_words))){
                if(res_w_names[i] == TRUE)
                    return_value = Token(TRUE, true, source.get_line(), source.get_pos());
                if(res_w_names[i] == FALSE)
                    return_value = Token(FALSE, false, source.get_line(), source.get_pos());
                return_value = Token(res_w_names[i], reserved_words[i], source.get_line(), source.get_pos());
            } else
                return_value = Token(IDENTIFIER, buffer, source.get_line(), source.get_pos());
            cur_state = _INIT;
            return false;
        case _OUT_INT:
            token_is_ready = true;
            source.unget_c(c);
            return_value = Token(INT, atoi(buffer.c_str()), source.get_line(), source.get_pos());
            cur_state = _INIT;
            return false;
        case _OUT_REAL0:
            token_is_ready = true;
            source.unget_c(c);
            return_value = Token(REAL, strtod(buffer.c_str(), 0), source.get_line(), source.get_pos());
            cur_state = _INIT;
            return false;
        case _OUT_STRING:
            token_is_ready = true;
            buffer.erase(buffer.begin());
            return_value = Token(STRING, buffer, source.get_line(), source.get_pos());
            cur_state = _INIT;
            return false;
        case _OUT_DELIMITER:
            source.unget_c(c);
            token_is_ready = true;
            if(c == -1 && buffer.empty())
                return_value = Token(ENDOF, 0, source.get_line(), source.get_pos());
            else {
                i = search(buffer, signs);
                return_value = Token(sig_names[i], signs[i], source.get_line(), source.get_pos());
            }
            cur_state = _INIT;
            return false;
        case _UNTERM_STR_ERR:
            throw bad_jaba(E_LEXER, "unterminated string", source.get_line(), source.get_pos());
        case _UNKNOWN_CHAR_ERR:
            throw bad_jaba(E_LEXER, "unsupported symbol", source.get_line(), source.get_pos());
        case _BAD_REAL0_ERR:
            throw bad_jaba(E_LEXER, "bad real value", source.get_line(), source.get_pos());
        default: ;
    }
    push();
    if(cur_state == _INIT)
        clear_buffer();
    c = smb == -1 ? next() : smb;
    cur_state = (state)DFA[type(c)][cur_state];
    return smb != -1;
}

Token Lexer::get_token(bool produce_if_not_ready){
    if(!token_is_ready && !produce_if_not_ready)
        throw bad_jaba(E_GLOBAL, "get_token(false) called while token is not ready");
    token_is_ready = false;
    while(!token_is_ready){
        use_next_smb();
    }
    pure = true;
    return return_value;
}

Token& Token::operator=(const Token &source){
    type = source.type;
    v_string = source.v_string;
    v_int = source.v_int;
    v_double = source.v_double;
    v_bool = source.v_bool;
    line = source.line;
    pos = source.pos;
    return *this;
}
