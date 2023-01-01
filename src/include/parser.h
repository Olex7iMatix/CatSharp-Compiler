#ifndef PARSER
#define PARSER

#include "token.h"
#include "lexer.h"
#include "AST.h"

typedef struct PARSER_STRUCT {
    lexer_T* lexer;
    token_T* current_token;
    token_T* previous_token;
} parser_T;

parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int type);

AST_T* parser_parse(parser_T* parser);

AST_T* parser_parse_expr(parser_T* parser);

AST_T* parser_parse_list(parser_T* parser);

AST_T* parser_parse_block(parser_T* parser);

AST_T* parser_parse_compound(parser_T* parser);

AST_T* parser_parse_id(parser_T* parser);

AST_T* parser_parse_int(parser_T* parser);

AST_T* parser_parse_vdef(parser_T* parser);

#endif