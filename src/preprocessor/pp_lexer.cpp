#include "preprocessor/pp_lexer.h"
#include <iostream>

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

void pp_lexer::check_escape_sequence(pp_lexer_results &results)
{
    if (is_oct(peek_next())) {
        for (size_t i = 0; i < 3; i++) {
            if (!is_oct(next()))
                break;
        }
        return;
    }

    switch (next()) {
        case '\'':
        case '\"':
        case '\?':
        case '\\':
        case 'a':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
        case 'v':
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

void pp_lexer::tokenize_pp_number(pp_lexer_results &results)
{
    if ((m_source[m_pos] == '.' && !is_digit(peek_next())) && !is_digit(m_source[m_pos])) {
        results.errors.emplace_back("Wrong number format!", m_line_offset, m_line);
        return;
    }

    for (char current = m_source[m_pos]; !is_end(); current = next()) {
        if ((current == 'e' || current == 'p' || current == 'E' || current == 'P') && (peek_next() == '+' || peek_next() == '-')) {
            next(); // an additional next call to capture sign
            continue;
        }

        if (current == '.' || is_alphanum(current))
            continue;

        break;
    }

    results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos - m_suboffset), pp_token_type::pp_number, m_line_offset, next_is_first_in_line);
}

void pp_lexer::tokenize_pp_identifier(pp_lexer_results &results)
{
    if (m_source[m_pos] != '_' && !is_alpha(m_source[m_pos])) {
        results.errors.emplace_back("Wrong identifier format!", m_line_offset, m_line);
        return;
    }

    for (char current = m_source[m_pos]; !is_end(); current = next()) {
        if (current == '_' || is_alpha(current))
            continue;

        if (current == '\\' && (peek_next() == 'u')) {
            next();
            for (size_t i = 0; i < 4; i++) {
                if (!is_hex(next())) {
                    results.errors.emplace_back("Wrong identifier format!", m_line_offset, m_line);
                    return;
                }
            }
            continue;
        }

        if (current == '\\' && (peek_next() == 'U')) {
            next();
            for (size_t i = 0; i < 8; i++) {
                if (!is_hex(next())) {
                    results.errors.emplace_back("Wrong identifier format!", m_line_offset, m_line);
                    return;
                }
            }
            continue;
        }

        break;
    }

    std::string lexeme = m_source.substr(m_suboffset, m_pos - m_suboffset);
    results.tokens.emplace_back(lexeme, keywords_token_table.contains(lexeme) ? keywords_token_table.at(lexeme) : pp_token_type::pp_identifier, m_line_offset, next_is_first_in_line);
}

void pp_lexer::tokenize_pp_char_constant(pp_lexer_results &results)
{
    if (m_source[m_pos] != 'L' && m_source[m_pos] != 'l' && m_source[m_pos] != 'u' && m_source[m_pos] != '\'') {
        results.errors.emplace_back("Wrong character constant!", m_line_offset, m_line);
        return;
    }

    if (m_source[m_pos] != '\'')
        next();
    for (char current = next(); true; current = next()) {
        if (current == '\'') {
            next();
            break;
        }

        if (current == '\\') {
            check_escape_sequence(results);
        }
    }

    results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos - m_suboffset), pp_token_type::pp_char_constant, m_line_offset, next_is_first_in_line);
}

void pp_lexer::tokenize_pp_string_literal(pp_lexer_results &results)
{
    if (m_source[m_pos] != 'L' && m_source[m_pos] != 'U' && m_source[m_pos] != 'u' && m_source[m_pos] != '\"') {
        results.errors.emplace_back("Invalid string literal!", m_line_offset, m_line);
        return;
    }

    if (m_source[m_pos] == 'u' && peek_next() == '8') {
        next();
    }

    if (m_source[m_pos] != '\"' && next() != '\"') {
        results.errors.emplace_back("Invalid string literal!", m_line_offset, m_line);
        return;
    }

    for (char current = next(); !is_end(); current = next()) {
        if (current == '\"') {
            next();
            break;
        }

        if (current == '\\') {
            check_escape_sequence(results);
        }
    }

    results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos - m_suboffset), pp_token_type::pp_char_constant, m_line_offset, next_is_first_in_line);
}

void pp_lexer::tokenize_pp_header_name(pp_lexer_results &results)
{
    if (m_source[m_pos] != '<' && m_source[m_pos] == '\"')
        results.errors.emplace_back("Invalid header name!", m_line_offset, m_line);

    if (m_source[m_pos] == '\"') {
        for (char current = next(); !is_end(); current = next()) {
            if (current == '\n')
                results.errors.emplace_back("Invalid header name!", m_line_offset, m_line);

            if (current == '\"') {
                next();
                break;
            }
        }
    }

    if (m_source[m_pos] == '<') {
        for (char current = next(); !is_end(); current = next()) {
            if (current == '\n')
                results.errors.emplace_back("Invalid header name!", m_line_offset, m_line);

            if (current == '>') {
                next();
                break;
            }
        }
    }

    skip_spaces();

    if (m_source[m_pos] == '\n')
        results.errors.emplace_back("Unexpected token after header name!", m_line_offset, m_line);

    results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos - m_suboffset - 1), pp_token_type::pp_header_name, m_line_offset, next_is_first_in_line);
}

pp_lexer_results pp_lexer::tokenize_from_source(std::string &&source)
{
    m_source = std::move(source);

    m_pos = 0;
    m_line = 1;
    m_suboffset = 0;
    m_line_offset = 0;

    pp_lexer_results result {};

    for (char current = m_source[m_pos]; !is_end();) {
        skip_spaces();
        m_line_offset = m_pos - m_suboffset;
        m_suboffset = m_pos;
        current = m_source[m_pos];

        if (result.tokens.size() > 0 && result.tokens.rbegin()->type == pp_token_type::k_include) {
            tokenize_pp_header_name(result);
            continue;
        }

        switch (current) {
            case '\n':
                //                m_line_offset = 0;
                //                next_is_first_in_line = true;
                break;

            default:

                if (is_digit(current)) {
                    tokenize_pp_number(result);
                    continue;
                }

                if (current == '\'' || peek_next() == '\'') {
                    tokenize_pp_char_constant(result);
                    continue;
                }

                if (current == '\"' || peek_next() == '\"' || (current == 'u' && peek_next() == '8' && peek(2) == '\"')) {
                    tokenize_pp_string_literal(result);
                    continue;
                }

                tokenize_pp_identifier(result);
                continue;
        }

        if (m_source[m_pos] != '\n')
            next_is_first_in_line = false;
        else {
            m_line_offset = 0;
            next_is_first_in_line = true;
        }

        next();
    }

    return result;
}

std::string tokens_to_text(std::vector<pp_token> &tokens)
{
    std::string result;
    for (auto &token : tokens) {
        if (token.first_in_line) {
            result += "\n";
            for (size_t i = 0; i < token.line_offset; i++) {
                result += " ";
            }
        }
        result += token.lexem;
    }
    return result;
}

};
