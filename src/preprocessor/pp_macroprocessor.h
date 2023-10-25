#ifndef PP_MACROPROCESSOR_H
#define PP_MACROPROCESSOR_H

#include "preprocessor/pp_token.h"
#include <unordered_map>
#include <vector>

namespace ctc::preprocessor
{

    struct pp_macroprocessor_results
    {
        std::vector<pp_token> tokens;
    };

    struct pp_obj_macro
    {
        std::vector<pp_token> tokens;
    };

    // struct pp_func_macro {
    //     // std::string identifier;
    //     std::vector<pp_token> tokens;
    // };

    struct pp_macroprocessor_intermediate_results
    {
        std::unordered_map<std::string, pp_obj_macro> obj_macros{};
    };

    class pp_macroprocessor
    {
        std::vector<pp_token> m_tokens;
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
            return (m_pos + 1 >= m_tokens.size()) ? m_tokens[m_tokens.size() - 1]
                                                  : m_tokens[m_pos + 1];
        }

        bool is_end()
        {
            return m_pos >= m_tokens.size();
        }

        pp_obj_macro parse_obj_macro();

        void parse_macro(pp_macroprocessor_intermediate_results &intermediate);
        void subprocess_tokens(std::vector<pp_token> &tokens);
        void parse_line(pp_macroprocessor_results &result,
                        pp_macroprocessor_intermediate_results &intermediate);

      public:
        pp_macroprocessor_results process_tokens(std::vector<pp_token> &&tokens);
    };

}; // namespace ctc::preprocessor

#endif // PP_MACROPROCESSOR_H
