#ifndef PP_TOKEN_H
#define PP_TOKEN_H

#include <string>

namespace ctc::preprocessor {

enum class pp_token_type {
    // preprocessing_tokens - pp_
    pp_include,
    pp_header_name, // = q-char-sequence or h-char-sequence
    pp_number,
    pp_identifier,
    pp_char_constant,
    pp_string_literal,

    // preprocessing keywords - k_
    k_include,
    k_define,

    // punctuators - p_
    p_leftbracket, // [
    p_rightbracket, // ]
    p_leftparentheses, // (
    p_rightparentheses, // )
    p_leftbrace, // {
    p_rightbrace, // }
    p_dot, // .
    p_arrow, // ->
    p_inc, // ++
    p_dec, // ++
    p_ampersand, // &
    p_star, // *
    p_plus, // +
    p_minus, // -
    p_tilde, //~
    p_exclmark, // !
    p_div, // /
    p_percentage, // %
    p_leftshift, // <<
    p_rightshift, // >>
    p_leftgreater, // <
    p_rightgreater, // >
    p_leftgreater_and_equal, // <=
    p_rightgreater_and_equal, // >=
    p_equalequal, // ==
    p_notequal, // !=
    p_xor, // ^
    p_and, // |
    p_double_ampersand, // &&
    p_double_and, // ||
    p_question, // ?
    p_colon, // :
    p_semicolon, // ;
    p_tripledot, // ...

    p_equal, // =
    p_mulequal, // *=
    p_divequal, // /=
    p_percequal, // %=
    p_plusequal, // +=
    p_minusequal, // -=
    p_leftshift_equal, // <<=
    p_rightshift_equal, // >>=
    p_amperequal, // &=
    p_xorequal, // ^=
    p_and_equal, // |=
    p_comma, // ,

    p_hash, // #
    p_double_hash, // ##,

    eof

};

struct pp_token {
    std::string lexem;
    pp_token_type type;

    size_t line_offset;
    bool first_in_line;
};

};

#endif // PP_TOKEN_H
