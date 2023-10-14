#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace ctc::tokenizer {

enum class token_type {
    // keywords - k_
    k_auto,
    k_break,
    k_case,
    k_char,
    k_const,
    k_continue,
    k_default,
    k_do,
    k_double,
    k_else,
    k_enum,
    k_extern,
    k_float,
    k_for,
    k_goto,
    k_if,
    k_inline,
    k_int,
    k_long,
    k_return,
    k_short,
    k_signed,
    k_sizeof,
    k_static,
    k_struct,
    k_switch,
    k_typedef,
    k_union,
    k_unsigned,
    k_void,
    k_volatile,
    k_while,
    k_bool,

    // punctuators - p_
    p_leftbracket,
    p_rightbracket, // [ and ]
    p_leftparentheses,
    p_rightparentheses, // ( and )
    p_leftbrace,
    p_rightbrace, // { and }
    p_dot,
    p_arrow, // . and ->
    p_inc,
    p_dec, // ++ and --
    p_ampersand,
    p_star, // & and *
    p_plus,
    p_minus, // + and -
    p_tilde,
    p_exclmark, // ~ and !
    p_div,
    p_percentage, // / and %
    p_leftshift,
    p_rightshift, // << and >>
    p_leftgreater,
    p_rightgreater, // < and >
    p_leftgreater_and_equal,
    p_rightgreater_and_equal, // <= and >=
    p_equalequal,
    p_notequal, // == and !=
    p_xor,
    p_and, // ^ and |
    p_double_ampersand,
    p_double_and, // && and ||
    p_question,
    p_colon, // ? and :
    p_semicolon,
    p_tripledot, // ; and ...

    p_equal,
    p_mulequal, // = and *=
    p_divequal,
    p_percequal, // /= and %=
    p_plusequal,
    p_minusequal, // += and -=
    p_leftshift_equal,
    p_rightshift_equal, // <<= and >>=
    p_amperequal,
    p_xorequal, // &= and ^=
    p_and_equal,
    p_comma, // |= and ,

    // literals - l_
    l_true,
    l_false,
    l_char_literal,
    l_string_literal,
    l_number_literal,

    eof // End of file
};

struct token {
    token_type type;
    std::string lexeme;

    explicit inline token(token_type type_of_token, std::string&& lexeme_of_token = "")
        : type(type_of_token)
        , lexeme(std::move(lexeme_of_token))
    {
    }

    std::string get_text() const noexcept; // Return field type as std::string
};

};

#endif // TOKEN_H
