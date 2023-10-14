#include "lexer.h"

#include <unordered_map>

namespace ctc::lexer
{
    static bool is_digit(char ch)
    {
        return isdigit(static_cast<unsigned char>(ch));
    }

    static bool is_alpha(char ch)
    {
        return isalpha(static_cast<unsigned char>(ch));
    }

    static bool is_alpha_or_digit(char ch)
    {
        return (is_alpha(ch) | is_digit(ch));
    }

    static std::unordered_map<std::string, token_type> keywords_token_table
    {
        {"auto", token_type::k_auto},
        {"break", token_type::k_break},

        {"case", token_type::k_case},
        {"char", token_type::k_char},

        {"const", token_type::k_const},
        {"continue", token_type::k_continue},
        {"default", token_type::k_default},

        {"do", token_type::k_do},
        {"double", token_type::k_double},

        {"else", token_type::k_else},
        {"enum", token_type::k_enum},
        {"extern", token_type::k_extern},

        {"float", token_type::k_float},
        {"for", token_type::k_for},
        {"goto", token_type::k_goto},

        {"if", token_type::k_if},
        {"inline", token_type::k_inline},
        {"int", token_type::k_int},
        {"long", token_type::k_long},

        {"return", token_type::k_return},
        {"short", token_type::k_short},
        {"signed", token_type::k_signed},
        {"sizeof", token_type::k_sizeof},

        {"static", token_type::k_static},
        {"struct", token_type::k_struct},
        {"switch", token_type::k_switch},
        {"typedef", token_type::k_typedef},

        {"union", token_type::k_union},
        {"unsigned", token_type::k_unsigned},
        {"void", token_type::k_void},
        {"volatile", token_type::k_volatile},

        {"while", token_type::k_while},
        {"bool", token_type::k_bool}
    };

    lexer_results lexer::tokenize_from_source(std::string&& source)
    {
        lexer_results results {};

        this->m_source = std::move(source);

        this->m_pos = 0;
        this->m_line = 1;

        while (!is_end())
        {
            this->m_suboffset = this->m_pos;

            char character = this->next();
            switch (character)
            {
                case '\t':
                case ' ':
                    break;

                case '\n':
                    this->m_line++;
                    break;

                case '"':
                    this->tokenize_string_literal(results);
                    break;

                // Punctuators
                case '[': results.tokens.emplace_back(token_type::p_leftbracket, "empty"); break;
                case ']': results.tokens.emplace_back(token_type::p_rightbracket, "empty"); break;

                case '(': results.tokens.emplace_back(token_type::p_leftparentheses, "empty"); break;
                case ')': results.tokens.emplace_back(token_type::p_rightparentheses, "empty"); break;

                case '{': results.tokens.emplace_back(token_type::p_leftbrace, "empty"); break;
                case '}': results.tokens.emplace_back(token_type::p_rightbrace, "empty"); break;

                case '.':

                    if ( peek(1) == '.' &&  peek(2) == '.' )
                    {
                        results.tokens.emplace_back(token_type::p_tripledot, "empty");

                        this->m_pos += 2;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_dot, "empty");
                    break;

                case '-':

                    if ( match('>') )
                    {
                        results.tokens.emplace_back(token_type::p_arrow, "empty");

                        this->m_pos++;
                        break;
                    }

                    if ( match('-') )
                    {
                        results.tokens.emplace_back(token_type::p_dec, "empty");

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_minusequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_minus, "empty");
                    break;

                case '+':

                    if ( match('+') )
                    {
                        results.tokens.emplace_back(token_type::p_inc, "empty");


                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_plusequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_plus, "empty");
                    break;

                case '&':

                    if ( match('&') )
                    {
                        results.tokens.emplace_back(token_type::p_double_ampersand, "empty");

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_amperequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_ampersand, "empty");
                    break;

                case '*':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_mulequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_star, "empty");
                    break;

                case '~': results.tokens.emplace_back(token_type::p_tilde, "empty"); break;

                case '!':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_notequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_exclmark, "empty");
                    break;

                case '/':

                    if ( match('/') )
                    {
                        while (!is_end() && peek(0) != '\n')
                            next();

                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_divequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_div, "empty");
                    break;

                case '%':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_percequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_percentage, "empty");
                    break;

                case '<':

                    if ( match('<') )
                    {
                        if ( peek(2) == '=' )
                        {
                            results.tokens.emplace_back(token_type::p_leftshift_equal, "empty");

                            this->m_pos += 2;
                            break;
                        }


                        results.tokens.emplace_back(token_type::p_leftshift, "empty");

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_leftgreater_and_equal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_leftgreater, "empty");
                    break;

                case '>':

                    if ( match('>') )
                    {
                        if ( peek(2) == '=' )
                        {
                            results.tokens.emplace_back(token_type::p_rightshift_equal, "empty");

                            this->m_pos += 2;
                            break;
                        }


                        results.tokens.emplace_back(token_type::p_rightshift, "empty");

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_rightgreater_and_equal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_rightgreater, "empty");
                    break;

                case '=':
                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_equalequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_equal, "empty");
                    break;

                case '^':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_xorequal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_xor, "empty");
                    break;


                case '|':

                    if ( match('|') )
                    {
                        results.tokens.emplace_back(token_type::p_double_or, "empty");

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_or_equal, "empty");

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_or, "empty");
                    break;

                case '?': results.tokens.emplace_back(token_type::p_question, "empty"); break;

                case ':': results.tokens.emplace_back(token_type::p_colon, "empty"); break;
                case ';': results.tokens.emplace_back(token_type::p_semicolon, "empty"); break;

                case ',': results.tokens.emplace_back(token_type::p_comma, "empty"); break;

                default:

                    if (is_digit(character))
                    {
                        tokenize_number_literal(results);
                        break;
                    }

                    if (is_alpha(character))
                    {
                        tokenize_identifier(results);
                        break;
                    }

                    results.errors.emplace_back("unrecognized character", m_line, m_pos);
                    break;
            }
        }

        return results;
    }

    void lexer::tokenize_string_literal(lexer_results& results) noexcept
    {
        while ( m_source[m_pos] != '"' && !is_end() )
        {
            if (m_source[m_pos] == '\n')
                m_line++;

            next();
        }

        if (is_end())
        {
            results.errors.emplace_back("incomplete string literal", m_line, m_pos);
            return;
        }

        next(); // skip the " symbol

        results.tokens.emplace_back(token_type::l_string_literal, m_source.substr(m_suboffset, m_pos - m_suboffset));
    }

    void lexer::tokenize_number_literal(lexer_results& results)
    {
        while ( is_digit(m_source[m_pos]) )
            next();

        if (m_source[m_pos] == '.' && !is_digit( peek_next() ))
        {
            results.errors.emplace_back( lexer_error { "Unable to process fractional literal", m_line, m_pos } );
            return;
        }

        if (m_source[m_pos] == '.')
            next();

        while ( is_digit(m_source[m_pos]) )
            next();

        results.tokens.emplace_back(token_type::l_number_literal, m_source.substr(m_suboffset, m_pos - m_suboffset));
    }

    void lexer::tokenize_identifier(lexer_results& results)
    {
        while ( is_alpha_or_digit(m_source[m_pos]) )
            next();

        std::string literal_value = m_source.substr(m_suboffset, m_pos - m_suboffset);

        results.tokens.emplace_back((keywords_token_table.contains(literal_value) ? keywords_token_table.at(literal_value) : token_type::identifier), std::move(literal_value));
    }
};
