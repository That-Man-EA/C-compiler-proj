#include "leocc.hpp"
#include <cassert>

/* 
Current CFG, with regex:
expr = mul ("+" mul | "-" mul)*
mul = primary ("*" primary | "/" primary)*
primary = "(" expr ")" 
        | num
num = <number>
*/
static Node* expr();
static Node* mul();
static Node* primary();
static Node* num();

static void print_expr_r(Node* _expr, int depth) {
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
    else {
        assert(false && "unreachable");//should never happen if code works correctly
    }

}

bool ast_debug = false;

void print_expr(Node* _expr) {
    print_expr_r(_expr, 0);
}

Node* parse() {
    tokens_i = 0;
    current_tok = tokens[tokens_i];
    
    Node* _expr = expr();

    if(ast_debug) {
        print_expr(_expr);
        exit(1);
    }
    
    return _expr;
}

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

    return _expr;
}

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
    if((current_tok->kind == TK_PUNCT) && (current_tok->punct == "(")){
        current_tok = tokens[++tokens_i];
        Node* _expr = expr();
        assert((current_tok->kind == TK_PUNCT) && (current_tok->punct == ")"));
        current_tok = tokens[++tokens_i];
        return _expr;
    }

    return num();
}
Node* num() {
    assert(current_tok->kind == TK_NUM);
    int n = current_tok->num;
    current_tok = tokens[++tokens_i];
    return new Node(ND_NUM, n);
}
