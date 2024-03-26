#include "leocc.hpp"
#include <cassert>

vector<Token*> tokens;
Token* current_tok = nullptr;
int tokens_i = 0;

void Token::print() {
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

void tokenize(char* p) {
    while(*p) {
        if(isspace(*p)) {
            p++; 
        }
        else if(isdigit(*p)) {
            char* q = p;
            long val = strtol(p, &p, 10);
            tokens.push_back(new Token(TK_NUM, val));
        }
        else if((*p == '+') || (*p == '-') || (*p == '*') || (*p == '/') || (*p == '(') || (*p == ')')){
            string s = "";
            s.push_back(*p);
            tokens.push_back(new Token(TK_PUNCT, s));
            p++;
        }
        else {
            assert(false && "shouldn't reach here");
        }
    }

    tokens.push_back(new Token(TK_EOF));
}