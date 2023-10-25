#include "preprocessor/pp_lexer.h"
#include <iostream>

using namespace ctc::preprocessor;
int main()
{
    pp_lexer lex{{}};

    auto result = lex.tokenize_from_source("1372.14.98e-10");
    auto &tokens = result.tokens;
    auto &errors = result.errors;
    std::string text = tokens_to_text(tokens);

    std::vector<pp_token> expected = {{"1372.14.98e-10", pp_token_type::pp_number, 0, false}};

    for (auto &tok : tokens)
    {
        std::cout << tok.lexem << " " << tok.line_offset << "|\n";
    }

    for (auto &err : errors)
    {
        std::cout << err.what << " " << err.line << " " << err.pos << "\n";
    }

    std::cout << text << "\n";

    bool passed = true;

    if (tokens.size() != expected.size())
    {
        std::cout << "Wrong number of tokens!\n";
        passed = false;
    }

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (expected[i].lexem != tokens[i].lexem ||
            expected[i].first_in_line != tokens[i].first_in_line ||
            expected[i].line_offset != tokens[i].line_offset || expected[i].type != tokens[i].type)
        {
            std::cout << "Wrong token: " << i << "!\n";
        }
    }

    if (text != "1372.14.98e-10 ")
        passed = false;

    if (!errors.empty())
        passed = false;

    return !passed;
}
