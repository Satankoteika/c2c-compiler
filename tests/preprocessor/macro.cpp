#include "preprocessor/pp_lexer.h"
#include "preprocessor/pp_macroprocessor.h"

int main()
{
    ctc::preprocessor::pp_macroprocessor proc {};
    ctc::preprocessor::pp_lexer lex { {} };
    auto tokens = lex.tokenize_from_source(
                         "#ifndef ABOBA\n #define ABOBA\n #ifdef ABOBA\n 1337\n #else\n 228\n"
                         "#endif\n #endif\n #endif\n #define MACRO1 123 "
                         "456\n #define MACRO MACRO1\n print MACRO macro\n #endif\n")
                      .tokens;
    auto tokens2 = lex.tokenize_from_source(
                          "#define IDENTITY(x, y, z, nigga, ...) #x kekestan "
                          "__VA_ARGS__ kekestan2 __VA_ARGS__\n "
                          "IDENTITY(1488 1337, aboba, biba, baba, ogromny huy)\n biba ##_NEGRA")
                       .tokens;

    proc.process_tokens(tokens2);
}
