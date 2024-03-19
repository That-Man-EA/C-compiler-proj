#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>
#include <list>

using namespace std;

/*
int main() {
    int a;
    return a + 5;
}

<kw: int>
<id: main>
<punct: (>
<punct: )>
<punct: {>
<kw: main> 
<id: a>
<kw: return>
<id: a>
<punct: +>
<num: 5>
*/

// 42
// 42 + 15 - 10
// [
// <num: 42>, 
// <punct: +>,
// <num: 15>,
// <punct: ->,
// <num: 10>
// ]

enum TokenKind {
    TK_NUM = 0,
    TK_PUNCT,
    TK_EOF
};

class Token {
public:
    TokenKind kind;

    Token(TokenKind kind, long val) : kind(kind), num(val) {

    };
    Token(TokenKind kind, string p) : kind(kind), punct(p) {

    };
    Token(TokenKind kind) : kind(kind) {}

    void print() {
        if(kind == TK_NUM) {
            cout << "<";
            cout << "TK_NUM: " << num;
            cout << ">" << endl;
        }
        else if(kind == TK_PUNCT) {
            cout << "<";
            cout << "TK_PUNCT: " << punct;
            cout << ">" << endl;
        }
        else if(kind == TK_EOF) {
            cout << "<";
            cout << "TK_EOF " << punct;
            cout << ">" << endl;
        }
    }

    // if TK_PUNCT
    string punct;

    // if TK_NUM
    int num;
};

vector<Token*> tokens;

void tokenize(char* p) {
    while(*p) {
        if(isspace(*p)) {
            p++;// do nothing 
        }
        else if(isdigit(*p)) {
            char* q = p;
            long val = strtol(p, &p, 10);
            tokens.push_back(new Token(TK_NUM, val));
        }
        else if((*p == '+') || (*p == '-')){
            // cout << "*p is " << *p << endl;
            string s = "";
            s.push_back(*p);
            // cout << "s is " << s << endl;
            tokens.push_back(new Token(TK_PUNCT, s));
            p++;
        }
        else {
            assert(false && "shouldn't reach here");
        }
    }

    tokens.push_back(new Token(TK_EOF));
}

// expected input: <some number>
int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("wrong number of arguments\n");
        exit(1);
    }

    tokenize(argv[1]);

    // for(int i = 0; i < tokens.size(); i++) {
    //     tokens[i]->print();
    // }

        
    cout << ".global _main" << endl;
    cout << "_main:" << endl;
    cout << "mov w0,#" << (*tokens.begin())->num << endl;


    int i = 1;
    Token* current = tokens[i];
    
    while(current->kind != TK_EOF){
        assert(current->kind == TK_PUNCT);

        if(current->punct == "+") {
            current = tokens[++i]; // move it past the +
            cout << "add w0, w0, #" << current->num << endl;
            current = tokens[++i]; // move it past the num, which will be either TK_EOF or another TK_PUNCT
        }
        else if(current->punct == "-") {
            current = tokens[++i]; // move it past the +
            cout << "sub w0, w0, #" << current->num << endl;
            current = tokens[++i];
        }
        else {
            assert(false && "unreachable");
        }

    }

    cout << "ret" << endl;
    
    return 0;
}