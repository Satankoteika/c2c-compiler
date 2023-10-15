#include "preprocessor/pp_lexer.h"

namespace ctc::preprocessor {

static bool is_digit(char ch)
{
    return isdigit(static_cast<unsigned char>(ch));
}

static bool is_alpha(char ch)
{
    return isalpha(static_cast<unsigned char>(ch));
}

static bool is_alphanum(char ch)
{
    return isalnum(static_cast<unsigned char>(ch));
}

static bool is_hex(char ch)
{
    return isxdigit(static_cast<unsigned char>(ch));
}

static bool is_oct(char ch)
{
    return '0' > ch && ch < '8';
}

void pp_lexer::tokenize_pp_number(pp_lexer_results &results)
{
    if ((m_source[m_pos] == '.' && !is_digit(peek_next())) && !is_digit(m_source[m_pos])) {
        results.errors.emplace_back("Wrong number format!", m_pos, m_line);
        return;
    }

    for (char current = m_source[m_pos]; true; current = next()) {
        if (current == '.' || is_alphanum(current))
            continue;

        if ((current == 'e' || current == 'p' || current == 'E' || current == 'P') && (peek_next() == '+' || peek_next() == '-'))
            continue;

        break;
    }

    results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos), pp_token_type::pp_number, m_line, m_pos);
}

void pp_lexer::tokenize_pp_identifier(pp_lexer_results &results)
{
    if (m_source[m_pos] != '_' && !is_alpha(m_source[m_pos])) {
        results.errors.emplace_back("Wrong identifier format!", m_pos, m_line);
        return;
    }

    for (char current = m_source[m_pos]; true; current = next()) {
        if (current == '_' || is_alpha(current))
            continue;

        if (current == '\\' && (peek_next() == 'u')) {
            next();
            for (size_t i = 0; i < 4; i++) {
                if (!is_hex(next())) {
                    results.errors.emplace_back("Wrong identifier format!", m_pos, m_line);
                    return;
                }
            }
            continue;
        }

        if (current == '\\' && (peek_next() == 'U')) {
            next();
            for (size_t i = 0; i < 8; i++) {
                if (!is_hex(next())) {
                    results.errors.emplace_back("Wrong identifier format!", m_pos, m_line);
                    return;
                }
            }
            continue;
        }

        break;
    }

    results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos), pp_token_type::pp_identifier, m_line, m_pos);
}

void pp_lexer::tokenize_pp_char_constant(pp_lexer_results &results)
{
    if (m_source[m_pos] != 'L' && m_source[m_pos] != 'l' && m_source[m_pos] != 'u' && m_source[m_pos] != '\'') {
        results.errors.emplace_back("Wrong character constant!", m_pos, m_line);
        return;
    }

    if (m_source[m_pos] != '\'')
        next();
    for (char current = next(); true; current = next()) {
        if (current == '\'')
            break;

        if (current == '\\') {
            if (is_oct(peek_next())) {
                for (size_t i = 0; i < 3; i++) {
                    if (!is_oct(next()))
                        break;
                }
                continue;
            }

            switch (next()) {
                case '\'':
                case '\"':
                case '\?':
                case '\\':
                    break;

                case 'x':
                    if (!(is_hex(next()) && is_hex(next())))
                        results.errors.emplace_back("Wrong character constant!", m_pos, m_line);
                    break;

                default:
                    results.errors.emplace_back("Wrong character constant!", m_pos, m_line);
                    break;
            }
        }
    }
}

// void pp_lexer::tokenize_pp_string_literal(){}

};
