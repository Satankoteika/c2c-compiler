#ifndef PP_LEXER_H
#define PP_LEXER_H

#include "preprocessor/pp_token.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ctc::preprocessor {

struct pp_lexer_options { };

struct pp_lexer_error {
    std::string what;
    size_t pos, line;
};

struct pp_lexer_results {
    std::vector<pp_lexer_error> errors;
    std::vector<pp_token> tokens;
};

class pp_lexer {
    inline static const std::unordered_map<std::string, pp_token_type> keywords_token_table {
        std::make_pair("include", pp_token_type::k_include),
        std::make_pair("define", pp_token_type::k_define)
    };

    std::string m_source;
    size_t m_pos, m_line, m_suboffset;

    inline bool is_space(char ch)
    {
        return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
    }

    inline void skip_spaces()
    {
        while (is_space(peek_next())) {
            next();
        }

        if (is_space(m_source[m_pos]))
            next();
    }

    inline char next()
    {
        return m_source[m_pos++];
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

    inline bool is_end() const
    {
        return (m_pos >= m_source.length());
    }

    void tokenize_pp_number(pp_lexer_results &results);
    void tokenize_pp_identifier(pp_lexer_results &results);
    void tokenize_pp_char_constant(pp_lexer_results &results);
    void tokenize_pp_string_literal(pp_lexer_results &results);
    void tokenize_pp_header_name(pp_lexer_results &results);
};

};

#endif // PP_LEXER_H