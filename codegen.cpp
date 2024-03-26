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

static void codegen_expr(Node* _expr) {
    assert(_expr);
    if(_expr->kind == ND_ADD) {
        codegen_expr(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen_expr(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  add %rbx, %rax" << endl;
    }
    else if(_expr->kind == ND_SUB) {
        codegen_expr(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen_expr(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  sub %rbx, %rax" << endl;
    }
    else if(_expr->kind == ND_NUM) {
        cout << "  mov $" << _expr->num << ", %rax" << endl;
    }
    else if(_expr->kind == ND_MUL) {
        codegen_expr(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen_expr(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  imul %rbx, %rax" << endl;
    }
    else if(_expr->kind == ND_DIV) {
        codegen_expr(_expr->rhs);
        cout << "  push %rax" << endl;
        codegen_expr(_expr->lhs);
        cout << "  pop %rbx" << endl;
        cout << "  mov $0, %rdx" << endl;
        cout << "  div %rbx" << endl;
    }
    else {
        assert(false && "unreachable");
    }
}