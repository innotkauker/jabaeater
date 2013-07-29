#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "bad_jaba.h"
#include "filesource.h"
#include "local_types.h"

using std::ostream;
using std::string;
using std::cout;
using std::clog;
using std::endl;

const int BUFSIZE = 128;

class Token{
    token_type type;
    string v_string;
    int v_int;
    double v_double;
    bool v_bool;
    int line;
    int pos;
public:
    Token(){}
    Token(const Token &source): type(source.type), v_string(source.v_string), v_int(source.v_int), v_double(source.v_double), v_bool(source.v_bool), line(source.line), pos(source.pos) {}
    Token(token_type new_type, int new_value, int new_line = -1, int new_pos = -1): type(new_type), v_int(new_value), line(new_line), pos(new_pos) {}
    Token(token_type new_type, double new_value, int new_line = -1, int new_pos = -1): type(new_type), v_double(new_value), line(new_line), pos(new_pos) {}
    Token(token_type new_type, bool new_value, int new_line = -1, int new_pos = -1): type(new_type), v_bool(new_value), line(new_line), pos(new_pos) {}
    Token(token_type new_type, string new_value, int new_line = -1, int new_pos = -1): type(new_type), v_string(new_value), line(new_line), pos(new_pos) {}
    ~Token(){}
    Token &operator=(const Token &source);
    token_type get_type(){return type;}
    int get_int_value(){return v_int;}
    double get_real_value(){return v_double;}
    bool get_bool_value(){return v_bool;}
    string get_string_value(){return v_string;}
    int get_line(){return line;}
    int get_pos(){return pos;}
    void set_type(token_type t){type = t;}
    void set_int_v(int v){v_int = v;}
    friend ostream& operator<< (ostream &out, Token what);
    void print(ostream &where);
};

enum state{ /// DFA states
    _INIT, _LEXEME, _INT, _STRING1, _DIV, _REAL3,
    _TRIPLE, _DOUBLE, _SINGLE, _COMMENT,
    _REAL0,
    _OUT_LEX, _OUT_INT,
    _OUT_REAL0, _OUT_STRING, _UNTERM_STR_ERR,
    _OUT_DELIMITER, _UNKNOWN_CHAR_ERR, _BAD_REAL0_ERR
};

enum symbol { /// classes of symbols DFA works with
    __SPACE, __NEWLN, __EOF,
    __CHAR, /// without 'e' and 'E', but with '_'
    __DIGIT, __APOS1, __SLASH,
    __EQU, __DOUBLE,
    __DOT, __NOT, __OTHER,
    __UNRECOGNIZED
};

class Lexer{
    static int DFA[][11];
    static string reserved_words[];
    static string signs[];
    static token_type res_w_names[];
    static token_type sig_names[];

    state cur_state;
    FileSource source;
    char c;
    bool pure; /// true if the process of getting new token has ended
               /// and it was read by someone outside
    bool token_is_ready;
    Token return_value;

    string buffer; /// stores whatever lexer read during current pass
    void clear_buffer(){buffer.clear();}
    void push(){buffer += c;}

    int search(const string needle, const string *haystack);
                    /// searches in static tables

    char next(){return c = source.get_next();}
    symbol type(char smb);
public:
    bool use_next_smb(char smb = -1); /// false is returned if smb was set
                                      /// but wasn't used
                                      /// next call should be without parameters then
    bool token_ready(){return token_is_ready;}
    Token get_token(bool produce_if_not_ready = true);
    Lexer(FileSource &file);
    ~Lexer(){}
};


#endif // LEXER_H
