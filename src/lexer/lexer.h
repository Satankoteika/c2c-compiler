#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <vector>

namespace ctc::lexer
{
    struct lexer_error
    {
        inline lexer_error(std::string&& error_what, size_t error_line, size_t error_pos)
                                                                                        :  what(std::move(error_what)), line(error_line), pos(error_pos) {}

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
        lexer_options options;

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

        void tokenize_string_literal(lexer_results& results) noexcept;
        void tokenize_number_literal(lexer_results& results);
        void tokenize_identifier(lexer_results& results);

    public:
        explicit inline lexer(lexer_options options_for_lexing) : options(options_for_lexing) {}

        lexer_results tokenize_from_source(std::string&& source);
    };
}

#endif

