#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// expected input: <some number>
int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("wrong number of arguments\n");
        exit(1);
    }
    
    cout << ".global _main" << endl;
    cout << "_main:" << endl;
    cout << "mov w0,#" << argv[1] << endl;
    cout << "ret" << endl;
    
    return 0;
}