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
Token* current_tok;
int tokens_i;


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

// Context Free Grammar (CFG)
// a set of terminals
// a set of non-terminals
// a start symbol
// rules for each nonterminal
// expr := primary primary_opt
// primary := "(" expr ")"
//          |  num
// primary_opt := "+" primary primary_opt
            //  | "-" primary primary_opt
            //  | epsilon
// expr := primary ("+" primary | "-" primary)*
// num is a terminal given by any set of digits
// our start symbol is expr
// expr =>
// primary "+" primary ("+" primary | "-" primary)*

// 5 + 10 - 5
// (5 + 10) - 5
    // +
// 5     -
//     10  5

// mov w0, 5   // 
// push        // [5, 10, 5]
// mov w0, 10
// push w0     // [5, 10]

// pop w0      //  [5, 10]
// pop w1      //  [5]
// sub w0, w1, w0
// push w0    //   [5, 5]
// mov w0, #5
// push w0      // [5]
// pop w0     // [5]
// pop w1      // []
// add w0, w1, w0
// ret
//     -  
//   +   5
// 5   10
// expr => 
// primary "+" primary =>
// num "+" primary 
// num "+" num 

// ND_NUM,
// ND_PRIMARY,
// ND_PRIMARY_OPT,
// ND_ADD,
// ND_SUB
// 
// show the path starting from expr to generate the program "115 - 30 + 110"
// expr =>                                                                  ND_EXPR
// primary primary_opt =>                                               ND_PRIM     ND_PRIM_OPT
// 115 primary_opt =>                                                       115
// 115 "-" primary primary_opt =>                                                    ND_PRIM        ND_PRIM_OPT
// 115 "-" 30 primary_opt =>                                                            30          
// 115 "-" 30 "+" primary primary_opt =>                                                        ND_PRIM  ND_PRIM_OPT
// 115 "-" 30 "+" 110 primary_opt =>                                                              110    
// 115 "-" 30 "+" 110 epsilon =>                                                                            epsilon
// 115 "-" 30 "+" 110 
//
//
//
//
//

// primary := num
// expr := primary ("+" primary | "-" primary)*
// num := <any integer>

// 42

enum NodeKind {
    ND_NUM,
    ND_ADD,
    ND_SUB
};

class Node {
public:
    NodeKind kind;

    // fields for ND_NUM
    int num;

    //fields for ND_ADD or ND_SUB
    Node *lhs, *rhs;

    // ND_NUM ctor
    Node(NodeKind kind, int n) : kind(kind), num(n) {}

    // ND_ADD or ND_SUB ctor
    Node(NodeKind kind, Node* l, Node* r) : kind(kind), lhs(l), rhs(r) {}
};

Node* expr();
Node* primary();
Node* num();

// 4+2-6 |
// expr := primary ("+" primary | "-" primary)*
Node* expr() {
    Node* _expr = primary();

    while(current_tok->kind != TK_EOF) {
        assert(current_tok->kind == TK_PUNCT);

        if(current_tok->punct == "+") {
            current_tok = tokens[++tokens_i];
            Node* _prim = primary();

            _expr = new Node(ND_ADD, _expr, _prim);
        }
        else if(current_tok->punct == "-") {
            current_tok = tokens[++tokens_i];
            Node* _prim = primary();

            _expr = new Node(ND_SUB, _expr, _prim);
        }
        else {
            assert(false && "unreachable");
        }
    }


    assert(current_tok->kind == TK_EOF);
    return _expr;
}

Node* primary() {
    return num();
}
// <TK_NUM: "42">
// num := <any integer>
Node* num() {
    assert(current_tok->kind == TK_NUM);
    int n = current_tok->num;
    current_tok = tokens[++tokens_i];
    return new Node(ND_NUM, n);
}

void codegen(Node* _expr) {
    assert(_expr);
    if(_expr->kind == ND_ADD) {
        codegen(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  add %rbx, %rax" << endl;
    }
    else if(_expr->kind == ND_SUB) {
        codegen(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  sub %rbx, %rax" << endl;
        
    }
    else if(_expr->kind == ND_NUM) {
        cout << "  mov $" << _expr->num << ", %rax" << endl;
    }
    else {
        assert(false && "unreachable");
    }
}


// expected input: <some number>
int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("wrong number of arguments\n");
        exit(1);
    }

    tokenize(argv[1]);

    for(int i = 0; i < tokens.size(); i++) {
        //tokens[i]->print();
    }

        
    cout << ".global main" << endl;
    cout << "main:" << endl;


    tokens_i = 0;
    current_tok = tokens[tokens_i];

    Node* top_expr = expr();
    
    codegen(top_expr);

    cout << "  ret" << endl;
    //4+2-6
    
    return 0;
}