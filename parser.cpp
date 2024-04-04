#include "leocc.hpp"
#include <cassert>

static Node* expr();
static Node* mul();
static Node* unary();
static Node* primary();
static Node* num();
static Node* add();
static Node* equality();
static Node* relational();
static Node* expr_stmt();
static Node* stmt();
static Node* program();

static const string begin_node = "|-- ";
static const string comment = "// ";
const static int width_per_depth = 2;

static void print_expr_r(Node* _expr, int depth) {
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
    else if(_expr->kind == ND_LT){
        cout << "<" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_LTE){
        cout << "<=" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_GT){
        cout << ">" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_GTE){
        cout << ">=" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_EQ){
        cout << "==" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else if(_expr->kind == ND_NEQ){
        cout << "!=" << endl;
        print_expr_r(_expr->lhs, depth + 1);
        print_expr_r(_expr->rhs, depth + 1);
    }
    else {
        assert(false && "unreachable");//should never happen if code works correctly
    }

}

bool ast_debug = false;
void print_expr(Node* _expr, int depth); 

void print_expr_stmt(Node* _expr_stmt, int depth){
    cout << comment;
    for(int i = 0; i < depth * width_per_depth; i++){
        cout << " ";
    }
    cout << begin_node << "EXPR_STMT" << endl;
    print_expr(_expr_stmt->_expr, depth+1);
}

void print_stmt(Node* _stmt, int depth){
    print_expr_stmt(_stmt, depth);
}

void print_program(Node* _program){
    assert(_program->kind == ND_PROGRAM);
    cout << comment << begin_node << "PROGRAM" << endl;
    for(Node* stmt : _program->stmtList) {
        print_stmt(stmt, 1);
    }
}

void print_expr(Node* _expr, int depth) {
    print_expr_r(_expr, depth);
}

Node* parse() {
    tokens_i = 0;
    current_tok = tokens[tokens_i];
    
    Node* _program = program();

    if(ast_debug) {
        print_program(_program);
        exit(1);
    }
    
    return _program;
}


/* 
Current CFG, with regex:
program = stmt*
stmt = expr-stmt
expr-stmt = expr ";"
expr = equality
equality = relational ("==" relational | "!=" relational)*
relational = add ("<" add | "<=" add | ">" add | ">=" add)*
add = mul ("+" mul | "-" mul)*
mul = unary ("*" unary | "/" unary)*
unary = ("+" | "-") unary | primary
primary = "(" expr ")" | num
*/
Node* program(){
    vector<Node*> stmtList;

    while(current_tok->kind != TK_EOF){
        Node* _stmt = stmt();
        stmtList.push_back(_stmt);
    }
    assert(current_tok->kind == TK_EOF);
    return new Node(ND_PROGRAM,stmtList);   
}

Node* stmt(){
    return expr_stmt();
}

Node* expr_stmt(){
    Node* _expr = expr();
    assert((current_tok->kind == TK_PUNCT) && (current_tok->punct == ";"));
    
    // advance the token pointer, just throw away/dont do anything with the ";"
    current_tok = tokens[++tokens_i];

    return new Node(ND_EXPR_STMT, _expr);
}

Node* expr(){
    return equality();
}

static bool is_equality_token(string punct) {
    return (punct == "==") || (current_tok->punct == "!=");
}
// equality = relational ("==" relational | "!=" relational)*
Node* equality(){
    Node* _expr = relational();
    while((current_tok->kind == TK_PUNCT) && is_equality_token(current_tok->punct)){
        if(current_tok->punct == "=="){
            current_tok = tokens[++tokens_i];
            _expr = new Node(ND_EQ,_expr,relational());
        }
        else if(current_tok->punct == "!="){
            current_tok = tokens[++tokens_i];
            _expr = new Node(ND_NEQ,_expr,relational());
        }
        else{
            assert(false && "unreachable");
        }

    }
    return _expr;
}

static bool is_relational_token(string punct) {
    return ((punct == "<") || (punct == "<=")|| (punct == ">")|| (punct == ">=")); 
}
Node* relational(){
    Node* _expr = add();
    while((current_tok->kind == TK_PUNCT) && is_relational_token(current_tok->punct)){
        if(current_tok->punct == "<="){
            current_tok = tokens[++tokens_i];
            _expr = new Node(ND_LTE, _expr, add());
        }
        else if(current_tok->punct == "<"){
            current_tok = tokens[++tokens_i];
            _expr = new Node(ND_LT, _expr, add());
        }
        else if(current_tok->punct == ">="){
            current_tok = tokens[++tokens_i];
            _expr = new Node(ND_GTE, _expr, add());
        }
        else if(current_tok->punct == ">"){
            current_tok = tokens[++tokens_i];
            _expr = new Node(ND_GT, _expr, add());
        }
        else{
            assert(false && "unreachable");
        }
    }


    return _expr;
}

Node* add() {
    Node* _expr = mul();//

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
    Node* _expr = unary();

    while((current_tok->punct == "*") || (current_tok->punct == "/") ) {
        assert(current_tok->kind == TK_PUNCT);

        if(current_tok->punct == "*") {
            current_tok = tokens[++tokens_i];
            Node* _prim = unary();

            _expr = new Node(ND_MUL, _expr, _prim);
        }
        else if(current_tok->punct == "/") {
            current_tok = tokens[++tokens_i];
            Node* _prim = unary();

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

Node* unary() {
    if(current_tok->kind == TK_PUNCT && (current_tok->punct == "+" || current_tok->punct == "-")) {
        // unary + does nothing. just return unary
        if(current_tok->punct == "+"){
            current_tok = tokens[++tokens_i];
            return unary();
        }
        //Unary minus multiplies expression by (-1)
        else if(current_tok->punct == "-"){
            current_tok = tokens[++tokens_i];
            return new Node(ND_MUL,
                            new Node(ND_NUM, -1),
                            unary());
        }
        else{
            assert(false && "unreachable");
        }
    }
    else {
        //this is the 2nd rule
        return primary();
    }
    
}