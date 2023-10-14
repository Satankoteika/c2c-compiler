#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "tokenizer/token.h"
#include <vector>

namespace ctc::tokenizer {

struct lexer_error {
    std::string what;
    size_t line, pos;
};

struct tokenizer_options {
    bool skip_errors;
};

struct tokenizer_results {
    std::vector<token> tokens;
    std::vector<lexer_error> errors;

    bool successfully { true };
};

class tokenizer {
private:
    tokenizer_options options;
    std::string m_source;
    size_t m_pos, m_line, m_suboffset;

    // Helpful methods
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

    void tokenize_single(tokenizer_results &results);

    void tokenize_string_literal(tokenizer_results &results) noexcept;
    void tokenize_number_literal();
    void tokenize_identifier();

public:
    explicit inline tokenizer(tokenizer_options options)
        : options(options)
    {
    }

    tokenizer_results tokenize_from_source(std::string &&source);
};

}

#endif // TOKENIZER_H
