#include "pp_macroprocessor.h"

namespace ctc::preprocessor {

pp_obj_macro pp_macroprocessor::parse_obj_macro()
{
    pp_obj_macro result;
    while (!is_end() && !peek_next().first_in_line) {
        result.tokens.push_back(next());
    }
    return result;
}

void pp_macroprocessor::parse_macro(pp_macroprocessor_intermediate_results &intermediate)
{
    if (peek_next().type == pp_token_type::p_leftparentheses) {
        return;
    }

    intermediate.obj_macros[m_tokens[m_pos].lexem] = parse_obj_macro();
}

void pp_macroprocessor::parse_line(
    pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate)
{

    if (m_tokens[m_pos].type == pp_token_type::p_hash && m_tokens[m_pos].first_in_line) {
        for (pp_token current = next(); !is_end(); next()) {
            switch (current.type) {
                case pp_token_type::k_define:
                    next();
                    parse_macro(intermediate);
                    break;
                default:
                    return;
            }
        }

        return;
    }

    for (pp_token current = m_tokens[m_pos]; !is_end() && !m_tokens[m_pos].first_in_line;
         current = next()) {

        if (m_tokens[m_pos].type == pp_token_type::pp_identifier
            && intermediate.obj_macros.contains(m_tokens[m_pos].lexem)) {
            for (auto &token : intermediate.obj_macros.at(m_tokens[m_pos].lexem).tokens) {
                result.tokens.push_back(token);
            }

            return;
        }

        result.tokens.push_back(current);
    }
}

pp_macroprocessor_results pp_macroprocessor::process_tokens(std::vector<pp_token> &&tokens)
{
    m_tokens = std::move(tokens);

    pp_macroprocessor_results result {};
    pp_macroprocessor_intermediate_results intermediate {};

    while (!is_end()) {
        parse_line(result, intermediate);
    }

    return result;
}

};
