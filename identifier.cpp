#include "identifier.h"

Identifier::Identifier(const Identifier &source){
    name = source.name;
    v_string = source.v_string;
    type = source.type;
    v_int = source.v_int;
    v_double = source.v_double;
    v_bool = source.v_bool;
}

//Identifier::Identifier(const Token &source){
//    switch(source.get_type()){
//        case IDENTIFIER:
//            name = source.name;
//            v_string = source.v_string;
//            type = source.type;
//            v_int = source.v_int;
//            v_double = source.v_double;
//            v_bool = source.v_bool;
//            break;
//        case BOOL:
//
//    }
//
//}

IdentifierStack::IdentifierStack(int depth, bool log){
    size = depth;
    data = new Identifier[depth];
    first_free = data;
    block_indentions.push(0);
    push(Identifier("Responce"));
    push(Identifier("Environment"));
    logging = log;
}

void IdentifierStack::push(Identifier what){
    if(first_free - data == size)
        throw bad_jaba(E_GLOBAL, "identifier stack overflow");
    *(first_free++) = what;
}

Identifier* IdentifierStack::get(string name){
    for(int i = first_free - data; i > 0; i--){
        if(data[i-1].get_name() == name)
            return data + i - 1;
    }
    return 0;
}

void IdentifierStack::drop(){
    if(block_indentions.size() <= 1)
        throw bad_jaba(E_GLOBAL, "attempt to drop the whole identifier stack");
    first_free = data + block_indentions.top();
    if(logging) clog << "block dropped @ " << block_indentions.top() << " from bottom" << endl;
    block_indentions.pop();
}

void IdentifierStack::set_block(){
    block_indentions.push(first_free - data);
    if(logging) clog << "block set @ " << first_free - data << " from bottom" << endl;
}

bool IdentifierStack::defined_in_cur_block(string name){
    for(int i = block_indentions.top(); i < first_free - data; i++)
        if(data[i].get_name() == name)
            return true;
    return false;
}
