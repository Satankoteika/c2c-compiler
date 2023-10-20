#include "preprocessor/pp_lexer.h"
#include <iostream>

using namespace ctc::preprocessor;
int main()
{
    pp_lexer lex { {} };
    auto tokens = lex.tokenize_from_source("1372.1498e-10 abc cba\n bca \"biba\" L\"boba\" "
                                           "U\"boba\" u\"boba\" u8\"boba\" \'\\xA1\'")
                      .tokens;
    for (auto &tok : tokens) {
        std::cout << tok.lexem << "|\n";
    }

    std::cout << tokens_to_text(tokens);

    return 0;
}
