#include "filesource.h"

FileSource::FileSource(const string &program, bool in_file){
    if(in_file){
        string_stream_used = false;
        src1 = new ifstream(program.c_str());
//        src1.open(program.c_str());
        source = src1;
    } else {
        string_stream_used = true;
        src2 = new stringstream(program);
//        src2.str(program);
        source = src2;
    }
    line_n = 1;
    pos_n = 0;
}

char FileSource::get_next(){
    char c = source -> get();
    if(c == '\n') {
        line_n++;
        pos_n = 0;
    }
    pos_n++;
    return c;
}

void FileSource::unget_c(char what){
    source -> putback(what);
    if(what == '\n')
        line_n--;
    pos_n--;
}
