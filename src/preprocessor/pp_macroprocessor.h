#ifndef PP_MACROPROCESSOR_H
#define PP_MACROPROCESSOR_H

#include "preprocessor/pp_token.h"
#include <unordered_map>
#include <vector>

namespace ctc::preprocessor {

struct pp_macroprocessor_results {
    std::vector<pp_token> tokens;
};

struct pp_macro {
    // std::string identifier;
    std::vector<pp_token> tokens;
};

class pp_macroprocessor {
    std::vector<pp_token> m_tokens;
<<<<<<< HEAD
    size_t m_pos;

    inline const pp_token &next()
    {
        m_pos++;
        if (m_pos >= m_tokens.size())
            return m_tokens[m_tokens.size() - 1];
        return m_tokens[m_pos];
    }

    inline const pp_token &peek(size_t index) const
    {
        return (m_pos + index >= m_tokens.size()) ? m_tokens[m_tokens.size() - 1]
                                                  : m_tokens[m_pos + index];
    }

    inline const pp_token &peek_next() const
    {
        return (m_pos + 1 >= m_tokens.size()) ? m_tokens[m_tokens.size() - 1] : m_tokens[m_pos + 1];
    }

    bool is_end() { return m_pos + 1 >= m_tokens.size(); }
=======

    void make_macro(size_t index, std::unordered_map<std::string, pp_macro> &macros);
>>>>>>> c2c-team-main

public:
    pp_macroprocessor_results process_tokens(std::vector<pp_token> &&tokens);
};

};

#endif // PP_MACROPROCESSOR_H
