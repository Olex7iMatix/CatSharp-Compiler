#include "include/as_frontend.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* as_f_root(AST_T* ast) {
    char* value = 0;
    char* next_value = 0;

    const char* section_text = ".section .text\n"
                               "call start\n"
                               "movl %eax, %ebx\n"
                               "movl $1, %eax\n"
                               "int $0x80\n\n";

    value = calloc((strlen(section_text) + 128), sizeof(char));
    sprintf(value, "%s", section_text);

    next_value = as_f(ast);

    char* new_value = calloc((strlen(next_value) + 1 + (strlen(section_text) + 128)) * sizeof(char),sizeof(char));
    sprintf(new_value, "%s%s", value, next_value);
    free(value);
    free(next_value);
    value = new_value;

    return value;
}

char* as_f(AST_T* ast) {
    char* value = 0;
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
    
    value = calloc((strlen(next_val) + 1), sizeof(char));

    strcat(value, next_val);
    free(next_val);

    return value;
}

char* as_f_compound(AST_T* ast) {
    char* val = 0;

    for (int i = 0; i < ast->childs_size; i++) {
        AST_T* child = ast->childs[i];
        if (child == (void*) 0) { continue; }
        char* next_val = as_f(child);
        val = realloc(val, (strlen(next_val) + 1) * sizeof(char));
        strcat(val, next_val);
        free(next_val);
    }

    if (val == 0) { return ""; }

    return val;
}

char* as_f_assigment(AST_T* ast) {
    char* s = 0;

    if (ast->value->type == AST_FUNCTION) {
        
        const char* template = ".globl %s\n"
                                "%s:\n";
        char* as_val_val = as_f(ast->value->value);
        int size = (strlen(template) + (strlen(ast->name) * 2) + 1 + strlen(as_val_val));
        s = calloc(size, sizeof(char));
        sprintf(s, template, ast->name, ast->name);
        strcat(s, as_val_val);
        free(as_val_val);
    }
    return s;
}

char* as_f_variable(AST_T* ast) {
    // char* s = 0;
    
    return "# UwU";
}

char* as_f_call(AST_T* ast) {
    char* s = 0;

    if (strcmp(ast->name, "return") == 0) {
        AST_T* first_arg = ast->value->childs_size != 0 ? ast->value->childs[0] : (void*) 0;
        const char* template = "movl $%d, %%eax\n"
                               "ret\n";
        s = calloc(strlen(template) + 1, sizeof(char));
        sprintf(s, template, first_arg ? first_arg->int_value : 0);
    } else {
        s = malloc(15);
        strcpy(s, "# UwU");
    }

    return s;
}

char* as_f_int(AST_T* ast) {
    char* s = 0;
    
    return s;
}