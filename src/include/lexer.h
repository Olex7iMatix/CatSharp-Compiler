#ifndef LEXER
#define LEXER

#include "token.h"

#include <stdio.h>

typedef struct LEXER_STRUCT {
    char* source;
    size_t source_len;

    char c;
    unsigned int i;
    int line;
} lexer_T;

lexer_T* init_lexer(char* src);

void lexer_advance(lexer_T* lexer);

char lexer_peek(lexer_T* lexer, int offset);

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token);

void lexer_skip_whitespace(lexer_T* lexer);

token_T* lexer_next_token(lexer_T* lexer);

token_T* lexer_collect_id(lexer_T* lexer);

token_T* lexer_collect_string(lexer_T* lexer);

token_T* lexer_collect_int(lexer_T* lexer);

#endif