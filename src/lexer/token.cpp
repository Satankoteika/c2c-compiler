#include <token.h>

#include <unordered_map>

namespace ctc::lexer
{
    static std::unordered_map<token_type, std::string> token_type_to_string_table
    {
        {token_type::k_auto, "k_auto"},
        {token_type::k_break, "k_break"},

        {token_type::k_case, "k_case"},
        {token_type::k_char, "k_char"},

        {token_type::k_const, "k_const"},
        {token_type::k_continue, "k_continue"},
        {token_type::k_default, "k_default"},

        {token_type::k_do, "k_do"},
        {token_type::k_double, "k_double"},

        {token_type::k_else, "k_else"},
        {token_type::k_enum, "k_enum"},
        {token_type::k_extern, "k_extern"},

        {token_type::k_float, "k_float"},
        {token_type::k_for, "k_for"},
        {token_type::k_goto, "k_goto"},

        {token_type::k_if, "k_if"},
        {token_type::k_inline, "k_inline"},
        {token_type::k_int, "k_int"},
        {token_type::k_long, "k_long"},

        {token_type::k_return, "k_return"},
        {token_type::k_short, "k_short"},
        {token_type::k_signed, "k_signed"},
        {token_type::k_sizeof, "k_sizeof"},

        {token_type::k_static, "k_static"},
        {token_type::k_struct, "k_struct"},
        {token_type::k_switch, "k_switch"},
        {token_type::k_typedef, "k_typedef"},

        {token_type::k_union, "k_union"},
        {token_type::k_unsigned, "k_unsigned"},
        {token_type::k_void, "k_void"},
        {token_type::k_volatile, "k_volatile"},

        {token_type::k_while, "k_while"},
        {token_type::k_bool, "k_bool"},
        {token_type::p_leftbracket, "p_leftbracket"},

        {token_type::p_rightbracket, "p_rightbracket"},
        {token_type::p_leftparentheses, "p_leftparentheses"},

        {token_type::p_rightparentheses, "p_rightparentheses"},
        {token_type::p_leftbrace, "p_leftbrace"},

        {token_type::p_rightbrace, "p_rightbrace"},
        {token_type::p_dot, "p_dot"},
        {token_type::p_arrow, "p_arrow"},
        {token_type::p_inc, "p_inc"},
        {token_type::p_dec, "p_dec"},

        {token_type::p_ampersand, "p_ampersand"},
        {token_type::p_star, "p_star"},
        {token_type::p_plus, "p_plus"},
        {token_type::p_minus, "p_minus"},
        {token_type::p_tilde, "p_tilde"},

        {token_type::p_exclmark, "p_exclmark"},
        {token_type::p_div, "p_div"},
        {token_type::p_percentage, "p_percentage"},
        {token_type::p_leftshift, "p_leftshift"},
        {token_type::p_rightshift, "p_rightshift"},

        {token_type::p_leftgreater, "p_leftgreater"},
        {token_type::p_rightgreater, "p_rightgreater"},
        {token_type::p_leftgreater_and_equal, "p_leftgreater_and_equal"},
        {token_type::p_rightgreater_and_equal, "p_rightgreater_and_equal"},
        {token_type::p_equalequal, "p_equalequal"},

        {token_type::p_notequal, "p_notequal"},
        {token_type::p_xor, "p_xor"},
        {token_type::p_and, "p_and"},
        {token_type::p_double_ampersand, "p_double_ampersand"},
        {token_type::p_double_and, "p_double_and"},
        {token_type::p_question, "p_question"},

        {token_type::p_colon, "p_colon"},
        {token_type::p_semicolon, "p_semicolon"},
        {token_type::p_tripledot, "p_tripledot"},
        {token_type::p_equal, "p_equal"},
        {token_type::p_mulequal, "p_mulequal"},

        {token_type::p_divequal, "p_divequal"},
        {token_type::p_percequal, "p_percequal"},
        {token_type::p_plusequal, "p_plusequal"},

        {token_type::p_minusequal, "p_minusequal"},
        {token_type::p_leftshift_equal, "p_leftshift_equal"},
        {token_type::p_rightshift_equal, "p_rightshift_equal"},

        {token_type::p_amperequal, "p_amperequal"},
        {token_type::p_xorequal, "p_xorequal"},
        {token_type::p_and_equal, "p_and_equal"},
        {token_type::p_comma, "p_comma"},

        {token_type::l_true, "l_true"},
        {token_type::l_false, "l_false"},
        {token_type::l_char_literal, "l_char_literal"},

        {token_type::l_string_literal, "l_string_literal"},
        {token_type::l_number_literal, "l_number_literal"},
        {token_type::eof, "eof"}
    };

    std::string ctc::lexer::token::get_text() const noexcept
    {
        return token_type_to_string_table.at(this->type);
    }
};
