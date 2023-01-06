#include "include/lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexer_T* init_lexer(char* src) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    lexer->source = src;
    lexer->source_len = strlen(src);
    lexer->i = 0;
    lexer->c = lexer->source[lexer->i];
    lexer->line = 1;

    return lexer;
}

void lexer_advance(lexer_T* lexer) {
    if (lexer->i < lexer->source_len && lexer->c != '\0') {
        lexer->i++;
        if (lexer->c == '\n') lexer->line++;
        lexer->c = lexer->source[lexer->i];
    }
}

char lexer_peek(lexer_T* lexer, int offset) {
    return lexer->source[lexer->i + offset];
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
    lexer_advance(lexer);

    return token;
}

void lexer_skip_whitespace(lexer_T* lexer) {
    while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' || lexer->c == '\t') {
        lexer_advance(lexer);
    }
}

token_T* lexer_next_token(lexer_T* lexer) {
    while (lexer->c != '\0') {
        if (iswspace(lexer->c) || lexer->c == 13 || lexer->c == 10) lexer_skip_whitespace(lexer);
        if (isdigit(lexer->c)) return lexer_collect_int(lexer);
        if (isalnum(lexer->c)) return lexer_collect_id(lexer);
        if (lexer->c == '"') return lexer_collect_string(lexer);
    
        switch (lexer->c)
        {
        case '(': return lexer_advance_with_token(lexer, init_token("(", TOKEN_LPAREN));
        case ')': return lexer_advance_with_token(lexer, init_token(")", TOKEN_RPAREN));
        case '<': return lexer_advance_with_token(lexer, init_token("<", TOKEN_LT));
        case '>': return lexer_advance_with_token(lexer, init_token(">", TOKEN_GT));
        case '{': return lexer_advance_with_token(lexer, init_token("{", TOKEN_LBRACE));
        case '}': return lexer_advance_with_token(lexer, init_token("}", TOKEN_RBRACE));
        case ',': return lexer_advance_with_token(lexer, init_token(",", TOKEN_COMMA));
        case '.': return lexer_advance_with_token(lexer, init_token(".", TOKEN_DOT));
        case ';': return lexer_advance_with_token(lexer, init_token(";", TOKEN_SEMI));
        case '=':
            if (lexer_peek(lexer, 1) == '=') { lexer_advance(lexer); return lexer_advance_with_token(lexer, init_token("==", TOKEN_EQUALS_EQUALS)); }
            else if (lexer_peek(lexer, 1) == '>') { lexer_advance(lexer); return lexer_advance_with_token(lexer, init_token("=>", TOKEN_EQUALS_GT)); }
            else if (lexer_peek(lexer, 1) == '<') { lexer_advance(lexer); return lexer_advance_with_token(lexer, init_token("=<", TOKEN_EQUALS_LT)); }
            else {
                return lexer_advance_with_token(lexer, init_token("=", TOKEN_EQUALS));
            }
        default: printf("[CatSharp] Error: Unexpected character on line %d: `%c`\n", lexer->line, lexer->c); exit(1);
        }
    }

    return init_token(0, TOKEN_EOF);
}

token_T* lexer_collect_id(lexer_T* lexer) {
    char* val = calloc(1, sizeof(char));
    
    while (isalnum(lexer->c)) {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]) {lexer->c, 0});
        lexer_advance(lexer);
    }

    return init_token(val, TOKEN_ID);
}

token_T* lexer_collect_string(lexer_T* lexer) {
    char* val = calloc(1, sizeof(char));

    lexer_advance(lexer);

    while (lexer->c != '"') {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]) {lexer->c, 0});
        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(val, TOKEN_STRING);
}

token_T* lexer_collect_int(lexer_T* lexer) {
    char* val = calloc(1, sizeof(char));
    
    while (isdigit(lexer->c)) {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]) {lexer->c, 0});
        lexer_advance(lexer);
    }

    return init_token(val, TOKEN_INT);
}