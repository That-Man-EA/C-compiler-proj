#include "leocc.hpp"
#include <cassert>

static void codegen_expr(Node* _expr);

void codegen(Node* _expr) {
    cout << ".global main" << endl;
    cout << "main:" << endl;

    codegen_expr(_expr);

    cout << "  ret" << endl;
    
    return;
}

static bool is_bin_op(Node* node){
    return (node->kind > ND_BINOP_BEGIN) && (node->kind < ND_BINOP_END);
}

static void codegen_expr(Node* _expr) {
    assert(_expr);
    if(is_bin_op(_expr)) {
        codegen_expr(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen_expr(_expr->lhs);
        cout << "  pop %rbx" << endl;

        if(_expr->kind == ND_ADD) {
            cout << "  add %rbx, %rax" << endl;
        }
        else if(_expr->kind == ND_SUB) {
            cout << "  sub %rbx, %rax" << endl;
        }
        else if(_expr->kind == ND_MUL) {
            cout << "  imul %rbx, %rax" << endl;
        }
        else if(_expr->kind == ND_DIV) {
            cout << "  mov $0, %rdx" << endl;
            cout << "  div %rbx" << endl;
        }
        else if(_expr->kind == ND_LT) {
            cout << "  cmp %rbx, %rax" << endl;
            cout << "  setl %al" << endl;
            cout << "  andb $1, %al" << endl;
            cout << "  movzb %al, %rax" << endl;
        }
        else if(_expr->kind == ND_LTE) {
            cout << "  cmp %rbx, %rax" << endl;
            cout << "  setle %al" << endl;
            cout << "  andb $1, %al" << endl;
            cout << "  movzb %al, %rax" << endl;
        }
        else if(_expr->kind == ND_GT) {
            cout << "  cmp %rbx, %rax" << endl;
            cout << "  setg %al" << endl;
            cout << "  andb $1, %al" << endl;
            cout << "  movzb %al, %rax" << endl;
        }
        else if(_expr->kind == ND_GTE) {
            cout << "  cmp %rbx, %rax" << endl;
            cout << "  setge %al" << endl;
            cout << "  andb $1, %al" << endl;
            cout << "  movzb %al, %rax" << endl;
        }
        else if(_expr->kind == ND_EQ) {
            cout << "  cmp %rbx, %rax" << endl;
            cout << "  sete %al" << endl;
            cout << "  andb $1, %al" << endl;
            cout << "  movzb %al, %rax" << endl;
        }
        else if(_expr->kind == ND_NEQ) {
            cout << "  cmp %rbx, %rax" << endl;
            cout << "  setne %al" << endl;
            cout << "  andb $1, %al" << endl;
            cout << "  movzb %al, %rax" << endl;
        }
    }
    else if(_expr->kind == ND_NUM) {
        cout << "  mov $" << _expr->num << ", %rax" << endl;
    }
    else {
        assert(false && "unreachable");
    }
}