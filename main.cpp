#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>
#include <list>

using namespace std;



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
        else if((*p == '+') || (*p == '-') || (*p == '*') || (*p == '/')){
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

enum NodeKind {
    ND_NUM,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV
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

/* 
Current CFG, with regex:
expr = mul ("+" mul | "-" mul)*
mul = primary ("*" primary | "/" primary)*
primary = "(" expr ")" 
        | num
num = <number>
*/
Node* expr();
Node* mul();
Node* primary();
Node* num();

void print_expr_r(Node* _expr, int depth) {
    static const string begin_node = "|-- ";
    static const string comment = "// ";
    const static int width_per_depth = 2;
    cout << comment;
    
    
    
    for(int i = 0; i < depth * width_per_depth; i++){
        cout << " ";
    }
    cout << begin_node;
    
    
    
    if(_expr->kind == ND_NUM){
        cout << _expr->num << endl;
        
    }      
    else if(_expr->kind == ND_ADD){
        cout << "+" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_SUB){
        cout << "-" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_MUL){
        cout << "*" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_DIV){
        cout << "/" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    

    else if(_expr->kind == ND_MUL) {
        
    }
/*
tree: 1
output:
1 

tree: +
    1   2
ND_PLUS
|--1
|--2

tree: 
         +
      +     3
    1   2
|--ND_PLUS
|==|--ND_PLUS
  |--ND_PLUS
   |--1
   |--2
 |--3
*/


}

void print_expr(Node* _expr) {
    print_expr_r(_expr, 0);
}
// |
// 2 * 3 + 4
//  ND_NUM: 2
// 4 + 2 * 3
// expr := primary ("+" primary | "-" primary | "*" primary | "/" primary)*

/*
leo@leovm:~/desktop/C-compiler-proj$ ./leocc --show-ast "2*3+4"
// |-- +
//   |-- *
//     |-- 2
//     |-- 3
//   |-- 4
// -> 
// |-- +
//   |-- 6
//   |-- 4
// -> 
// |-- 10
leo@leovm:~/desktop/C-compiler-proj$ ./leocc --show-ast "4+2*3"
// |-- *
//   |-- +
//     |-- 4
//     |-- 2
//   |-- 3
// -> 
// |-- *
//   |-- 6
//   |-- 3
// -> 
// |-- 18
*/
// expr = mul ("+" mul | "-" mul)*
// expr = primary ("+" primary | "-" primary)*


Node* expr() {
    Node* _expr = mul();

    while((current_tok->punct == "+") || (current_tok->punct == "-" )) {
        if(current_tok->punct == "+") {
            current_tok = tokens[++tokens_i];
            Node* _prim = mul();

            _expr = new Node(ND_ADD, _expr, _prim);
        }
        else if(current_tok->punct == "-") {
            current_tok = tokens[++tokens_i];
            Node* _prim = mul();

            _expr = new Node(ND_SUB, _expr, _prim);
        }
        else {
            assert(false && "unreachable");
        }
    }


    assert(current_tok->kind == TK_EOF);
    return _expr;
}

//mul = primary ("*" primary | "/" primary)*
Node* mul(){
    Node* _expr = primary();

    while((current_tok->punct == "*") || (current_tok->punct == "/") ) {
        assert(current_tok->kind == TK_PUNCT);

        if(current_tok->punct == "*") {
            current_tok = tokens[++tokens_i];
            Node* _prim = primary();

            _expr = new Node(ND_MUL, _expr, _prim);
        }
        else if(current_tok->punct == "/") {
            current_tok = tokens[++tokens_i];
            Node* _prim = primary();

            _expr = new Node(ND_DIV, _expr, _prim);
        }
        else {
            assert(false && "unreachable");
        }
    }

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
    else if(_expr->kind == ND_MUL) {
        codegen(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  imul %rbx, %rax" << endl;
        
    }
    else if(_expr->kind == ND_DIV) {
        codegen(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  mov $0, %rdx" << endl;
        cout << "  div %rbx" << endl;
        
    }
    else {
        assert(false && "unreachable");
    }
}

static bool debug = false;

// expected usage: leocc [--show-ast] <program>
int main(int argc, char* argv[]){
    char* input;

    if(argc == 3) {
        string flag = argv[1];
        input = argv[2];
        assert((flag == "--show-ast") && "only support --show-ast flag");
        debug = true;
    }
    else if(argc == 2) {
        input = argv[1];
    }
    else {
        printf("wrong number of arguments. expected usage: usage: leocc [--show-ast] <program>\n");
        exit(1);
    }

    tokenize(input);

    for(int i = 0; i < tokens.size(); i++) {
        //tokens[i]->print();
    }
  
    


    tokens_i = 0;
    current_tok = tokens[tokens_i];

    Node* top_expr = expr();

    if(debug) {
        print_expr(top_expr);
        exit(1);
    }
    
    
    cout << ".global main" << endl;
    cout << "main:" << endl;
    codegen(top_expr);

    cout << "  ret" << endl;
    //4+2-6
    
    return 0;
}