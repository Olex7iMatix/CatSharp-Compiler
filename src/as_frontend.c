#include "include/as_frontend.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* as_f_root(AST_T* ast) {
    char* next_value = 0;

    const char* section_text = ".section .text\n"
                               "jmp start\n"
                               "movl %eax, %ebx\n"
                               "movl $1, %eax\n"
                               "int $0x80\n\n";

    char* value = calloc((strlen(section_text) + 128), sizeof(char));
    strcpy(value, section_text);

    next_value = as_f(ast);

    value = realloc(value, (strlen(next_value) + 1) * sizeof(char));
    strcat(value, next_value);
    printf("%s\n", value);

    return value;
}

char* as_f(AST_T* ast) {
    char* value = calloc(1, sizeof(char));
    char* next_val = 0;

    switch (ast->type)
    {
    case AST_COMPOUND: next_val = as_f_compound(ast); break;
    case AST_FUNCTION: next_val = as_f_compound(ast->value); break;
    case AST_ASSIGMENT: next_val = as_f_assigment(ast); break;
    case AST_VARIABLE: next_val = as_f_variable(ast); break;
    case AST_CALL: next_val = as_f_call(ast); break;
    case AST_INT: next_val = as_f_int(ast); break;
    default: printf("[CatSharp] Error: Can't compile AST type `%i`\n", ast->type); exit(1);
    }
    
    value = realloc(value, (strlen(next_val) + 1) * sizeof(char));

    strcat(value, next_val);

    return value;
}

char* as_f_compound(AST_T* ast) {
    char* val = calloc(1, sizeof(char));

    for (int i = 0; i < ast->childs_size; i++) {
        AST_T* child = ast->childs[i];
        if (child == (void*) 0) { continue; }
        char* next_val = as_f(child);
        val = realloc(val, (strlen(next_val) + 1) * sizeof(char));
        strcat(val, next_val);
    }

    return val;
}

char* as_f_assigment(AST_T* ast) {
    char* s = calloc(1, sizeof(char));

    if (ast->value->type == AST_FUNCTION) {
        const char* template = ".globl %s\n"
                                "%s:\n";
        s = calloc(strlen(template) + (strlen(ast->name) * 2) + 1, sizeof(char));
        sprintf(s, template, ast->name, ast->name);
        AST_T* as_val = ast->value;

        char* as_val_val = as_f(as_val->value);
        s = realloc(s, (strlen(s) + strlen(as_val_val) + 1) * sizeof(char));
        strcat(s, as_val_val);
    }

    return s;
}

char* as_f_variable(AST_T* ast) {
    char* s = calloc(1, sizeof(char));
    
    return s;
}

char* as_f_call(AST_T* ast) {    
    char* s = calloc(1, sizeof(char));
    
    if (strcmp(ast->name, "return") == 0) {
        AST_T* first_arg = ast->value->childs_size != 0 ? ast->value->childs[0] : (void*) 0;
        const char* template = "movl $%d, %%eax\n"
                               "ret\n";
        char* ret_s = calloc(strlen(template) + 128, sizeof(char));
        sprintf(ret_s, template, first_arg ? first_arg->int_value : 0);
        s = realloc(s, (strlen(ret_s) + 1) * sizeof(char));
        strcat(s, ret_s);
    }

    return s;
}

char* as_f_int(AST_T* ast) {
    char* s = calloc(1, sizeof(char));
    
    return s;
}