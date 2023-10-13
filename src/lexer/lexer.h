#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <vector>

namespace ctc::lexer
{
    struct lexer_error
    {
        std::string what;

        size_t line, pos;
    };

    struct lexer_options
    {
        bool skip_errors;
    };

    struct lexer_results
    {
        std::vector<token> tokens;
        std::vector<lexer_error> errors;

        bool successfully {true};
    };

    class lexer
    {
    private:
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

            this->m_pos++;

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

    public:
        explicit lexer(lexer_options const options);

        lexer_results tokenize_from_source(std::string&& source);
    };
}

#endif
