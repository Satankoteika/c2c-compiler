#include "lexer.h"

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

    lexer_results ctc::lexer::lexer::tokenize_from_source(std::string&& source)
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

                // Punctuators
                case '[': results.tokens.emplace_back(token_type::p_leftbracket); break;
                case ']': results.tokens.emplace_back(token_type::p_rightbracket); break;

                case '(': results.tokens.emplace_back(token_type::p_leftparentheses); break;
                case ')': results.tokens.emplace_back(token_type::p_rightparentheses); break;

                case '{': results.tokens.emplace_back(token_type::p_leftbrace); break;
                case '}': results.tokens.emplace_back(token_type::p_rightbrace); break;

                case '.':

                    if ( peek(1) == '.' &&  peek(2) == '.' )
                    {
                        results.tokens.emplace_back(token_type::p_tripledot);

                        this->m_pos += 2;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_dot);
                    break;

                case '-':

                    if ( match('>') )
                    {
                        results.tokens.emplace_back(token_type::p_arrow);

                        this->m_pos++;
                        break;
                    }

                    if ( match('-') )
                    {
                        results.tokens.emplace_back(token_type::p_dec);

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_minusequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_minus);
                    break;

                case '+':

                    if ( match('+') )
                    {
                        results.tokens.emplace_back(token_type::p_inc);

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_plusequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_plus);
                    break;

                case '&':

                    if ( match('&') )
                    {
                        results.tokens.emplace_back(token_type::p_double_ampersand);

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_amperequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_ampersand);
                    break;

                case '*':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_mulequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_star);
                    break;

                case '~': results.tokens.emplace_back(token_type::p_tilde); break;

                case '!':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_notequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_exclmark);
                    break;

                case '/':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_divequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_div);
                    break;

                case '%':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_percequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_percentage);
                    break;

                case '<':

                    if ( match('<') )
                    {
                        if ( peek(2) == '=' )
                        {
                            results.tokens.emplace_back(token_type::p_leftshift_equal);

                            this->m_pos += 2;
                            break;
                        }


                        results.tokens.emplace_back(token_type::p_leftshift);

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_leftgreater_and_equal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_leftgreater);
                    break;

                case '>':

                    if ( match('>') )
                    {
                        if ( peek(2) == '=' )
                        {
                            results.tokens.emplace_back(token_type::p_rightshift_equal);

                            this->m_pos += 2;
                            break;
                        }


                        results.tokens.emplace_back(token_type::p_rightshift);

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_rightgreater_and_equal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_rightgreater);
                    break;

                case '=':
                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_equalequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_equal);
                    break;

                case '^':

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_xorequal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_xor);
                    break;

                case '|':

                    if ( match('|') )
                    {
                        results.tokens.emplace_back(token_type::p_double_and);

                        this->m_pos++;
                        break;
                    }

                    if ( match('=') )
                    {
                        results.tokens.emplace_back(token_type::p_and_equal);

                        this->m_pos++;
                        break;
                    }

                    results.tokens.emplace_back(token_type::p_and);
                    break;

                case '?': results.tokens.emplace_back(token_type::p_question); break;

                case ':': results.tokens.emplace_back(token_type::p_colon); break;
                case ';': results.tokens.emplace_back(token_type::p_semicolon); break;

                case ',': results.tokens.emplace_back(token_type::p_comma); break;

                default:
                    break;
            }
        }

        return results;
    }
};
