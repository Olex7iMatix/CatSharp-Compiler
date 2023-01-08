#include "include/parser.h"
#include "include/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer) {
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_next_token(lexer);
    parser->previous_token = parser->current_token;
}

void parser_eat(parser_T* parser, int type) {
    if (parser->current_token->type != type) {
        printf("[CatSharp Parser] Error: Unexpected token `%i` on line `%i`, expecting: `%i`\n", parser->current_token->type, parser->lexer->line, type);
        exit(1);
    }
    parser->previous_token = parser->current_token;
    parser->current_token = lexer_next_token(parser->lexer);
}

AST_T* parser_parse(parser_T* parser) {
    switch (parser->current_token->type)
    {
    case TOKEN_ID: return parser_parse_id(parser);
    case TOKEN_RBRACE: break;
    default: printf("[CatSharp] Error: Unexpected token `%i` on line `%i`\n", parser->current_token->type, parser->lexer->line); exit(1);
    }

    return (void*) 0;
}

AST_T* parser_parse_expr(parser_T* parser) {
    switch (parser->current_token->type)
    {
    case TOKEN_ID: return parser_parse_id(parser);
    case TOKEN_INT: return parser_parse_int(parser);
    case TOKEN_LPAREN: return parser_parse_list(parser);
    default: { printf("[CatSharp] Error: Unexpected token `%i` on line %d\n", parser->current_token->type, parser->lexer->line); exit(1); }
    }
}

AST_T* parser_parse_list(parser_T* parser) {
    parser_eat(parser, TOKEN_LPAREN);
    
    AST_T* ast = init_ast(AST_COMPOUND);

    if (parser->current_token->type != TOKEN_RPAREN) {
        ast->childs = calloc(1, sizeof(struct AST_STRUCT));
        ast->childs_size = 1;
        ast->childs[0] = parser_parse_expr(parser);

        while (parser->current_token->type == TOKEN_COMMA) {
            parser_eat(parser, TOKEN_COMMA);

            AST_T* child = parser_parse_expr(parser);
            if (ast->childs != NULL) {
                ast->childs_size++;
                ast->childs[ast->childs_size - 1] = child;
            }
        }
    } else {
        ast->childs = (void *) 0;
        ast->childs_size = 0;
    }

    parser_eat(parser, TOKEN_RPAREN);

    if (parser->current_token->type == TOKEN_EQUALS_GT) {
        parser_eat(parser, TOKEN_EQUALS_GT);
        ast->type = AST_FUNCTION;
        ast->value = parser_parse_block(parser);
    } else if (parser->current_token->type == TOKEN_LBRACE) {
        ast->type = AST_FUNCTION;

        ast->value = parser_parse_block(parser);
    }

    return ast;
}

AST_T* parser_parse_block(parser_T* parser) {
    parser_eat(parser, TOKEN_LBRACE);

    AST_T* ast = init_ast(AST_COMPOUND);

    while (parser->current_token->type != TOKEN_RBRACE) {
        AST_T* child = parser_parse(parser);
        
        if (ast->childs != NULL) {
            ast->childs_size++;
            ast->childs[ast->childs_size - 1] = child;
        } else {
            ast->childs = calloc(1, sizeof(struct AST_STRUCT));
            ast->childs_size = 1;
            ast->childs[0] = child;
        }

        parser_eat(parser, TOKEN_SEMI);
    }

    parser_eat(parser, TOKEN_RBRACE);

    return ast;
}

AST_T* parser_parse_compound(parser_T* parser) {
    AST_T* compoud = init_ast(AST_COMPOUND);

    while (parser->current_token->type != TOKEN_EOF) {
        AST_T* child = parser_parse(parser);
        
        if (compoud->childs != NULL) {
            compoud->childs_size++;
            compoud->childs[compoud->childs_size - 1] = child;
        } else {
            compoud->childs = calloc(1, sizeof(struct AST_STRUCT));
            compoud->childs_size = 1;
            compoud->childs[0] = child;
        }

        if (parser->current_token->type == TOKEN_SEMI)
            parser_eat(parser, TOKEN_SEMI);
    }

    return compoud;
}

AST_T* parser_parse_id(parser_T* parser) {
    char* id_value = calloc (strlen(parser->current_token->value) + 1, sizeof(char));
    strcpy(id_value, parser->current_token->value);
    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_LPAREN) {
        AST_T* ret_ast = init_ast(AST_CALL);
        ret_ast->value = parser_parse_list(parser);
        ret_ast->name = id_value;
        
        return ret_ast;
    } else {
        const char* type = id_value;

        id_value = calloc (strlen(parser->current_token->value) + 1, sizeof(char));
        strcpy(id_value, parser->current_token->value);
        parser_eat(parser, TOKEN_ID);

        if (parser->current_token->type == TOKEN_LPAREN) {
            AST_T* ret_ast = init_ast(AST_ASSIGMENT);
            ret_ast->value = parser_parse_list(parser);
            ret_ast->name = id_value;

            return ret_ast;
        }

        if (parser->current_token->type == TOKEN_EQUALS) {
            parser_eat(parser, TOKEN_EQUALS);
            AST_T* ast = init_ast(AST_ASSIGMENT);
            ast->name = id_value;
            ast->value = parser_parse_expr(parser);
            return ast;
        }

        AST_T* ast = init_ast(AST_VARIABLE);
        ast->name = id_value;
        ast->data_type = typename_to_int(type);
        return ast;
    }
}

AST_T* parser_parse_int(parser_T* parser) {
    int int_value = atoi(parser->current_token->value);
    parser_eat(parser, TOKEN_INT);
    AST_T* ast = init_ast(AST_INT);
    ast->int_value = int_value;
    return ast;
}