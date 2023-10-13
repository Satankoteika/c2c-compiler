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

                case '.': results.tokens.emplace_back(token_type::p_dot); break;

                case '-':

//                    if (match('>'))
//                            

                    break;

                default:
                    break;
            }
        }

        return results;
    }
};
