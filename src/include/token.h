#ifndef TOKEN
#define TOKEN

typedef struct TOKEN_STRUCT {
    char* value;
    enum {
        TOKEN_ID,
        TOKEN_STRING,
        TOKEN_INT,
        TOKEN_EQUALS,
        TOKEN_EQUALS_EQUALS,
        TOKEN_EQUALS_GT,
        TOKEN_EQUALS_LT,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LT,
        TOKEN_GT,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_COMMA,
        TOKEN_DOT,
        TOKEN_SEMI,
        TOKEN_EOF
    } type;
} token_T;

token_T* init_token(char* value, int type);

#endif