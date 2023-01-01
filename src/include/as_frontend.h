#ifndef AS_FRONTEND
#define AS_FRONTEND

#include "AST.h"

char* as_f(AST_T* ast);

char* as_f_root(AST_T* ast);

char* as_f_compound(AST_T* ast);

char* as_f_assigment(AST_T* ast);

char* as_f_variable(AST_T* ast);

char* as_f_call(AST_T* ast);

char* as_f_int(AST_T* ast);

#endif