#include "preprocessor/pp_lexer.h"
#include <iostream>

using namespace ctc::preprocessor;
int main()
{
    pp_lexer lex { {} };

    auto result
        = lex.tokenize_from_source("    _abcde1023bb_a\\u1234\\u2234c _abcde1023bb_a\\U12344321c");
    auto &tokens = result.tokens;
    auto &errors = result.errors;
    std::string text = tokens_to_text(tokens);

    std::vector<pp_token> expected
        = { { "_abcde1023bb_a\\u1234\\u2234c", pp_token_type::pp_identifier, 4, true },
              { "_abcde1023bb_a\\U12344321c", pp_token_type::pp_identifier, 32, false } };

    for (auto &tok : tokens) {
        std::cout << tok.lexem << " " << tok.line_offset << "|\n";
    }

    for (auto &err : errors) {
        std::cout << err.what << " " << err.line << " " << err.pos << "\n";
    }

    std::cout << text << "\n";

    bool passed = true;

    if (tokens.size() != expected.size()) {
        std::cout << "Wrong number of tokens!\n";
        passed = false;
    }

    for (size_t i = 0; i < std::min(expected.size(), tokens.size()); i++) {
        if (expected[i].lexem != tokens[i].lexem
            || expected[i].first_in_line != tokens[i].first_in_line
            || expected[i].line_offset != tokens[i].line_offset
            || expected[i].type != tokens[i].type) {
            std::cout << "Wrong token: " << i << "!\n";
            passed = false;
        }
    }

    if (text != "    _abcde1023bb_a\\u1234\\u2234c _abcde1023bb_a\\U12344321c ")
        passed = false;

    if (!errors.empty())
        passed = false;

    return !passed;
}
