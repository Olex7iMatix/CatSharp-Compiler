#include "include/cats.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/io.h"
#include "include/as_frontend.h"

void cats_compile(char* file, char* ofile) {
    lexer_T* lexer = init_lexer(getFileContent(file));
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse_compound(parser);
    char* s = as_f_root(root);
    printf("%s\n", s);
    writeToFile(ofile, s);
}