#include "preprocessor/pp_lexer.h"
#include <iostream>

using namespace ctc::preprocessor;
int main()
{
    pp_lexer lex { {} };
    auto result = lex.tokenize_from_source(
        "    \"aboba biba baba huy znayet cho tut pisat\" U\"aboba biba baba huy znayet cho "
        "tut pisat\" u\"aboba biba baba huy znayet cho tut pisat\" u8\"aboba biba baba "
        "huy znayet cho tut pisat\" L\"aboba biba baba huy znayet cho tut pisat КАК ЖЕ Я "
        "ЗАЕБАЛСЯ\"");
    auto &tokens = result.tokens;
    auto &errors = result.errors;
    auto text = tokens_to_text(tokens);

    std::vector<pp_token> expected = { { "\"aboba biba baba huy znayet cho tut pisat\"",
                                           pp_token_type::pp_string_literal, 4, true },
        { "U\"aboba biba baba huy znayet cho tut pisat\"", pp_token_type::pp_string_literal, 47,
            false },
        { "u\"aboba biba baba huy znayet cho tut pisat\"", pp_token_type::pp_string_literal, 91,
            false },
        { "u8\"aboba biba baba huy znayet cho tut pisat\"", pp_token_type::pp_string_literal, 135,
            false },
        { "L\"aboba biba baba huy znayet cho tut pisat КАК ЖЕ Я "
          "ЗАЕБАЛСЯ\"",
            pp_token_type::pp_string_literal, 180, false } };

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

    if (text
        != "    \"aboba biba baba huy znayet cho tut pisat\" U\"aboba biba baba huy znayet cho tut "
           "pisat\" u\"aboba biba baba huy znayet cho tut pisat\" u8\"aboba biba baba huy znayet "
           "cho tut pisat\" L\"aboba biba baba huy znayet cho tut pisat КАК ЖЕ Я ЗАЕБАЛСЯ\" ")
        passed = false;

    if (!errors.empty())
        passed = false;

    return !passed;
}
