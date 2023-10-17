#include "pp_macroprocessor.h"

namespace ctc::preprocessor {

void pp_macroprocessor::make_macro(size_t index, std::unordered_map<std::string, pp_macro> &macros)
{
    std::string identifier;
    pp_macro macro;

    if (m_tokens[index].type != pp_token_type::pp_identifier)
        return;

    identifier = m_tokens[index].lexem;
    index++;

    while (!m_tokens[index].first_in_line && index < m_tokens.size()) {
        macro.tokens.push_back(m_tokens[index]);
        index++;
    }

    macros[identifier] = macro;
}

pp_macroprocessor_results pp_macroprocessor::process_tokens(std::vector<pp_token> &&tokens)
{
    m_tokens = std::move(tokens);

    pp_macroprocessor_results result {};
    std::unordered_map<std::string, pp_macro> macros {};

    for (size_t i = 0; i < m_tokens.size(); i++) {
        if (i + 2 < m_tokens.size() && m_tokens[i].type == pp_token_type::p_hash
            && m_tokens[i].first_in_line) {
            i++;
            switch (m_tokens[i].type) {
                case pp_token_type::k_define:
                    make_macro(i + 1, macros);
                    break;

                default:
                    break;
            }
            continue;
        }

        if (m_tokens[i].type == pp_token_type::pp_identifier && m_tokens[i].lexem) {

            continue;
        }

        result.tokens.push_back(m_tokens[i]);
    }

    return result;
}

};
