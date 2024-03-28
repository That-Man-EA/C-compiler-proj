#include <iostream>
#include <vector>
#include <string>
using namespace std;


// BEGIN tokenizer declarations
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

    void print();

    // if TK_PUNCT
    string punct;

    // if TK_NUM
    int num;
};
extern vector<Token*> tokens;
extern Token* current_tok;
extern int tokens_i;

void tokenize(char* p);

// Begin parser declarations
enum NodeKind {
    ND_NUM = 0,
    ND_BINOP_BEGIN,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_LTE,
    ND_LT,
    ND_GTE,
    ND_GT,
    ND_EQ,
    ND_NEQ,
    ND_BINOP_END,
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

    // ND_ADD or ND_SUB  or ND_MUL or ND_DIV ctor
    Node(NodeKind kind, Node* l, Node* r) : kind(kind), lhs(l), rhs(r) {}
};

Node* parse();

void print_expr(Node* _expr);
extern bool ast_debug;

// Begin codegen declarations
void codegen(Node* _expr);