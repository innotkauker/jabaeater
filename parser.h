#include "rpn.h"
#include "lexer.h"
#include "bad_jaba.h"
#include "identifier.h"

class Parser{
    Token cur;
    Token object;
    Lexer *lexer;
    IdentifierStack id_st;
    RPN rpn;
    bool logging;

    Token next();
    void P();
    void PLO();
    void PL();
    void CS(int break_pos = -1, int cont_pos = -1);
    void Ss(int break_pos = -1, int cont_pos = -1);
    void S(int break_pos = -1, int cont_pos = -1);
    void EN(int ret_pos = -1);
    void C();
    void VOE();
    void Vs();
    void V();
    void EO();
    void E();
    void UE();
    void MeE(bool sub_exp = false);
    void PE();
    void ALO();

    bool cur_is_of(token_type type);
    bool Ident(bool var = false); /// checks if cur is an identifier
                                  /// and if it is declared or not
    bool any_operator();
public:
    Parser(Lexer *used_lexer, bool log = false): lexer(used_lexer), id_st(1024, log), logging(log) {}
    ~Parser(){}
    void parse();
    int get_cur_line(){return cur.get_line();}
    int get_cur_pos(){return cur.get_pos();}
    void set_logging(bool par){logging = par;}
    void out_rpn_array(){rpn.out_array();}
    RPN *get_rpn_el(){return &rpn;}
};
