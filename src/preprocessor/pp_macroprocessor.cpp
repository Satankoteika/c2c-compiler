#include "pp_macroprocessor.h"

namespace ctc::preprocessor
{

    pp_obj_macro pp_macroprocessor::parse_obj_macro()
    {
        pp_obj_macro result;
        for (pp_token current = m_tokens[m_pos]; !is_end(); current = m_tokens[m_pos])
        {
            result.tokens.push_back(current);

            if (next().first_in_line)
                break;
        }
        return result;
    }

    void pp_macroprocessor::parse_macro(pp_macroprocessor_intermediate_results &intermediate)
    {
        if (m_tokens[m_pos].type != pp_token_type::pp_identifier)
        {
            return;
        }

        if (peek_next().type == pp_token_type::p_leftparentheses)
        {
            return;
        }

        std::string macro_name = m_tokens[m_pos].lexem;
        intermediate.obj_macros[macro_name] = parse_obj_macro();
    }

    void pp_macroprocessor::parse_line(pp_macroprocessor_results &result,
                                       pp_macroprocessor_intermediate_results &intermediate)
    {

        if (m_tokens[m_pos].type == pp_token_type::p_hash && m_tokens[m_pos].first_in_line)
        {
            pp_token current = next();

            switch (current.type)
            {
                case pp_token_type::k_define:
                    next();
                    parse_macro(intermediate);
                    break;
                default:
                    return;
            }

            return;
        }

        for (pp_token current = m_tokens[m_pos]; !is_end(); current = m_tokens[m_pos])
        {
            if (m_tokens[m_pos].type == pp_token_type::pp_identifier &&
                intermediate.obj_macros.contains(m_tokens[m_pos].lexem))
            {
                for (auto &token : intermediate.obj_macros.at(m_tokens[m_pos].lexem).tokens)
                {
                    result.tokens.push_back(token);
                }
            }
            else
                result.tokens.push_back(current);

            if (next().first_in_line)
                return;
        }
    }

    pp_macroprocessor_results pp_macroprocessor::process_tokens(std::vector<pp_token> &&tokens)
    {
        m_tokens = std::move(tokens);

        pp_macroprocessor_results result{};
        pp_macroprocessor_intermediate_results intermediate{};

        while (!is_end())
        {
            parse_line(result, intermediate);
        }

        return result;
    }

}; // namespace ctc::preprocessor
