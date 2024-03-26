#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>
#include <list>

#include "leocc.hpp"

using namespace std;




// expected usage: leocc [--show-ast] <program>
int main(int argc, char* argv[]){
    char* input;

    if(argc == 3) {
        string flag = argv[1];
        input = argv[2];
        assert((flag == "--show-ast") && "only support --show-ast flag");
        ast_debug = true;
    }
    else if(argc == 2) {
        input = argv[1];
    }
    else {
        printf("wrong number of arguments. expected usage: usage: leocc [--show-ast] <program>\n");
        exit(1);
    }

    tokenize(input);
    Node* top_expr = parse();
    codegen(top_expr);

    return 0;
}