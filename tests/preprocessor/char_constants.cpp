#include "preprocessor/pp_lexer.h"
#include <iostream>

using namespace ctc::preprocessor;
int main()
{
    pp_lexer lex { {} };

    auto result = lex.tokenize_from_source(
        "    'a' 'z' 'f' 'd' u'a' U'z' L'f' L'я' '\\\'' '\\\"' '\\\?' '\\\\' '\\a' '\\b' '\\f' "
        "'\\n' '\\r' '\\t' '\\v' '\\0' '\\6' '\\55' '\\167' '\\xAA' '1337890'");
    auto &tokens = result.tokens;
    auto &errors = result.errors;
    std::string text = tokens_to_text(tokens);

    std::vector<pp_token> expected = {
        { "'a'", pp_token_type::pp_char_constant, 4, true }, // default symbols
        { "'z'", pp_token_type::pp_char_constant, 8, false },
        { "'f'", pp_token_type::pp_char_constant, 12, false },
        { "'d'", pp_token_type::pp_char_constant, 16, false },
        { "u'a'", pp_token_type::pp_char_constant, 20, false },
        { "U'z'", pp_token_type::pp_char_constant, 25, false },
        { "L'f'", pp_token_type::pp_char_constant, 30, false },
        { "L'я'", pp_token_type::pp_char_constant, 35, false },
        { "'\\\''", pp_token_type::pp_char_constant, 41, false }, // escape sequences
        { "'\\\"'", pp_token_type::pp_char_constant, 46, false },
        { "'\\\?'", pp_token_type::pp_char_constant, 51, false },
        { "'\\\\'", pp_token_type::pp_char_constant, 56, false },
        { "'\\a'", pp_token_type::pp_char_constant, 61, false },
        { "'\\b'", pp_token_type::pp_char_constant, 66, false },
        { "'\\f'", pp_token_type::pp_char_constant, 71, false },
        { "'\\n'", pp_token_type::pp_char_constant, 76, false },
        { "'\\r'", pp_token_type::pp_char_constant, 81, false },
        { "'\\t'", pp_token_type::pp_char_constant, 86, false },
        { "'\\v'", pp_token_type::pp_char_constant, 91, false },
        { "'\\0'", pp_token_type::pp_char_constant, 96, false },
        { "'\\6'", pp_token_type::pp_char_constant, 101, false },
        { "'\\55'", pp_token_type::pp_char_constant, 106, false },
        { "'\\167'", pp_token_type::pp_char_constant, 112, false },
        { "'\\xAA'", pp_token_type::pp_char_constant, 119, false },
        { "'1337890'", pp_token_type::pp_char_constant, 126, false }, // very very long char
    };

    // std::vector<pp_lexer_error> expected_errors = {};

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
        != "    'a' 'z' 'f' 'd' u'a' U'z' L'f' L'я' '\\\'' '\\\"' '\\\?' '\\\\' '\\a' '\\b' "
           "'\\f' "
           "'\\n' '\\r' '\\t' "
           "'\\v' '\\0' '\\6' '\\55' '\\167' '\\xAA' '1337890' ")
        passed = false;

    if (!errors.empty())
        passed = false;

    return !passed;
}
