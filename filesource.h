#ifndef FILESOURCE_H
#define FILESOURCE_H

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include "bad_jaba.h"

using std::ifstream;
using std::stringstream;
using std::istream;
using std::string;

/// allows to read from *some_mighty_source*
class FileSource{
    ifstream *src1;
    stringstream *src2;
    istream *source;
    bool string_stream_used;
    int line_n;
    int pos_n; /// in line
public:
    FileSource(){}
    FileSource(const string &program, bool in_file = true);
    ~FileSource(){}
    void unget_c(char what);
    int get_line(){return line_n;}
    int get_pos(){return pos_n;}
    char get_next();
};

#endif // FILESOURCE_H
