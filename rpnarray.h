#ifndef RPNARRAY_H
#define RPNARRAY_H

#include <vector>

using std::vector;

class RPNarray{
    vector<Token> data;
//    Token *data;
//    Token *first_free;
//    int size;
public:
    RPNarray(){}
    ~RPNarray(){}
    void set(Token what, int where = -1);
    Token &operator[](int index){return data[index];}
};

#endif // RPNARRAY_H
