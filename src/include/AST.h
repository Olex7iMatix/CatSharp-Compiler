#ifndef AST
#define AST

#include <stdio.h>

typedef struct AST_STRUCT {
    enum {
        AST_COMPOUND,
        AST_FUNCTION,
        AST_CALL,
        AST_ASSIGMENT,
        AST_DEFINITION_TYPE,
        AST_VARIABLE,
        AST_STATEMENT,
        AST_INT,
        AST_STRING,
        AST_NOOP
    } type;

    struct AST_STRUCT** childs;
    size_t childs_size;
    char* name;
    struct AST_STRUCT* value;
    int data_type;

    // AST_INT
    int int_value;
    // AST_STRING
    char* string_value;
} AST_T;

AST_T* init_ast(int type);

#endif