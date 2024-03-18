#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>

using namespace std;

// expected input: <some number>
int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("wrong number of arguments\n");
        exit(1);
    }
    stringstream ss(argv[1]);
    int num;
    ss >> num;

        
    cout << ".global _main" << endl;
    cout << "_main:" << endl;
    cout << "mov w0,#" << num << endl;




    char op;
    while(ss >> op) {
        ss >> num;
        if(op == '+') {
            cout << "add w0, w0, #" << num << endl;
        }
        else if(op == '-') {
            cout << "sub w0, w0, #" << num << endl;
        }
        else {
            assert(false && "expecting op + or -");
        }
    }

    cout << "ret" << endl;
    
    return 0;
}