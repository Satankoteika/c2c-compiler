#ifndef PP_LEXER_H
#define PP_LEXER_H

#include "preprocessor/pp_common.h"
#include "preprocessor/pp_token.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ctc::preprocessor {

struct pp_lexer_options { };

struct pp_lexer_results {
    std::vector<pp_error> errors;
    std::vector<pp_token> tokens;
};

class pp_lexer {
    inline static const std::unordered_map<std::string, pp_token_type> keywords_token_table {
        std::make_pair("include", pp_token_type::k_include),
        std::make_pair("define", pp_token_type::k_define),
        std::make_pair("ifdef", pp_token_type::k_ifdef),
        std::make_pair("ifndef", pp_token_type::k_ifndef),
        std::make_pair("elifdef", pp_token_type::k_elifdef),
        std::make_pair("elifndef", pp_token_type::k_elifndef),
        std::make_pair("endif", pp_token_type::k_endif),
        std::make_pair("defined", pp_token_type::k_defined),
        std::make_pair("__VA_ARGS__", pp_token_type::k_va_args),
    };

    pp_lexer_options options;
    std::string m_source;
    size_t m_pos, m_line, m_suboffset;

    size_t m_line_offset;
    bool first_in_line = false;

    inline bool is_space(char ch)
    {
        return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
    }

    inline void skip_spaces()
    {
        while (is_space(m_source[m_pos])) {
            next();
        }
    }

    inline char next()
    {
        m_pos++;
        m_line_offset++;

        if (m_pos >= m_source.length())
            return '\0';

        return m_source[m_pos];
    }

    inline bool match(char to_find)
    {
        if (is_end() || m_source.at(m_pos) != to_find)
            return false;

        return true;
    }

    inline char peek(size_t index) const
    {
        return (m_pos + index >= m_source.length()) ? '\0' : m_source.at(m_pos + index);
    }

    inline char peek_next() const
    {
        return (m_pos + 1 >= m_source.length()) ? '\0' : m_source.at(m_pos + 1);
    }

    inline bool is_end() const { return (m_pos >= m_source.length()); }

    inline void add_token(pp_lexer_results &results, pp_token_type type)
    {
        results.tokens.emplace_back(m_source.substr(m_suboffset, m_pos - m_suboffset + 1), type,
            m_line_offset - (m_pos - m_suboffset), first_in_line);
    }

    void check_escape_sequence(pp_lexer_results &results);

    void tokenize_pp_number(pp_lexer_results &results);
    void tokenize_pp_identifier(pp_lexer_results &results);
    void tokenize_pp_char_constant(pp_lexer_results &results);
    void tokenize_pp_string_literal(pp_lexer_results &results);
    void tokenize_pp_header_name(pp_lexer_results &results);

    void tokenize_line(pp_lexer_results &results);

public:
    pp_lexer_results tokenize_from_source(std::string &&source);

    pp_lexer(pp_lexer_options options)
        : options(options)
    {
    }
};

std::string tokens_to_text(std::vector<pp_token> &tokens);

};

#endif // PP_LEXER_H
