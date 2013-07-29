#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "executioner.h"

using namespace std;

int main(){
    /// lexer
//    try{
//        FileSource input("test1.js");
////        FileSource input("function a()", false);
//        Lexer lexer(input);
//        Token tmp;
//        while(true){
//            if((tmp = lexer.get_token()).get_type() == ENDOF) break;
//            cout << tmp;
//        }
//    }
//    catch(bad_jaba err){
//        cerr << err << endl << "stopping" << endl;
//    }

    /// parser & reverse polish notation
//    try{
//        FileSource input("test0.js");
//        Lexer lexer(input);
//        Parser parser(&lexer, true);
//        parser.parse();
//        parser.out_rpn_array(); /// die "POLIZ"
//    }
//    catch(bad_jaba err){
//        cerr << err << endl << "stopping" << endl;
//    }
//    clog << "done." << endl;

    /// execution
    try{
        FileSource input("test0.js");
        Lexer lexer(input);
        Parser parser(&lexer, false);
        parser.parse();
        parser.out_rpn_array();
        Executioner executor(parser.get_rpn_el());
        cout << endl << "program returned " << executor.execute() << endl;
    }
    catch(bad_jaba err){
        cerr << err << endl << "stopping" << endl;
    }
    catch(exception err){
        cerr << err.what() << endl;
    }
    catch(...){
        cerr << "unknown error" << endl;
    }

//    int i;
//    cin >> i;
    return 0;
}
