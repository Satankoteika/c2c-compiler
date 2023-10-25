#include "pp_macroprocessor.h"
#include "preprocessor/pp_lexer.h"
#include <iostream>
#include <list>
#include <set>

namespace ctc::preprocessor {

pp_token pp_macroprocessor::evaluate_stringify(
    std::list<pp_token>::iterator first, std::list<pp_token>::iterator last)
{
    std::string result = "\"";
    for (auto iter = first; iter != last; iter++) {
        auto &token = *iter;

        if (token.type == pp_token_type::pp_char_constant
            || token.type == pp_token_type::pp_string_literal) {
            std::string updated;
            for (char c : token.lexem) {
                if (c == '\\' || c == '\"')
                    updated += '\\';

                updated += c;
            }
            result += updated;
        }

        result += token.lexem;
        if (std::next(iter) != last)
            result += " ";
    }

    result += "\"";
    return { result, pp_token_type::pp_string_literal, 0, false };
}

std::optional<pp_token> pp_macroprocessor::evaluate_concat(pp_token &first, pp_token &second)
{
    auto concat_result = pp_lexer { {} }.tokenize_from_source(first.lexem + second.lexem);
    if (concat_result.errors.size() == 0 && concat_result.tokens.size() == 1) {
        return { concat_result.tokens[0] };
    }

    return {};
}

void pp_macroprocessor::skip(pp_macroprocessor_results &result)
{
    for (; !is_end(); next()) {
        if (!current->first_in_line || current->type != pp_token_type::p_hash)
            continue;

        switch (peek_next().type) {
            case pp_token_type::k_ifdef:
            case pp_token_type::k_ifndef:
            case pp_token_type::k_else:
            case pp_token_type::k_endif:
                return;

            default:
                break;
        }
    }

    if (is_end()) {
        result.errors.emplace_back("Next directive not found!");
    }
}

void pp_macroprocessor::parse_func_like(
    pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate)
{
    std::unordered_map<std::string, size_t> arg_lexems;
    bool variadic = false;

    std::string macro_name = current->lexem;
    next_in_line();

    for (; next_in_line().type != pp_token_type::eol;) {

        switch (current->type) {
            case pp_token_type::pp_identifier:
                if (peek_next_in_line().type != pp_token_type::p_comma
                    && peek_next_in_line().type != pp_token_type::p_rightparentheses) {
                    result.errors.emplace_back("Missing comma!");
                    return;
                }

                if (arg_lexems.contains(current->lexem)) {
                    result.errors.emplace_back("Function-like macro argument name is duplicated!");
                    return;
                }

                arg_lexems[current->lexem] = arg_lexems.size();
                break;

            case pp_token_type::p_tripledot:
                if (!variadic)
                    variadic = true;
                else {
                    result.errors.emplace_back("Variadic arguments is duplicated!");
                    return;
                }

                if (peek_next_in_line().type != pp_token_type::p_rightparentheses) {
                    result.errors.emplace_back(
                        "Variadic arguments is not in the end of argument list!");
                    return;
                }
                break;

            default:
                result.errors.emplace_back("Unexpected token in macro declaration!");
                break;
        }

        next_in_line();
        if (current->type == pp_token_type::p_rightparentheses)
            break;
    }

    if (current->type != pp_token_type::p_rightparentheses) {
        result.errors.emplace_back("Missing closing parentheses!");
        return;
    }

    next_in_line();

    std::vector<pp_func_macro_arg> arg_positions;
    std::vector<pp_token> tokens;

    bool prev_hash = false;
    for (size_t offset = 0;; offset++) {

        if (current->type == pp_token_type::pp_identifier && arg_lexems.contains(current->lexem)) {
            arg_positions.emplace_back(offset, arg_lexems.at(current->lexem),
                std::prev(current)->type == pp_token_type::p_hash);
            prev_hash = false;
        }

        if (prev_hash) {
            result.errors.emplace_back("Hash operator is not followed with macro argument!");
            return;
        }

        if (current->type == pp_token_type::p_hash) {
            prev_hash = true;
            offset--;
        } else {
            tokens.push_back(*current);
        }

        if (next().first_in_line)
            break;
    }

    if (prev_hash) {
        result.errors.emplace_back("Hash operator is not followed with macro argument!");
        return;
    }

    intermediate.func_macros[macro_name] = { variadic, arg_lexems.size(), arg_positions, tokens };
}

void pp_macroprocessor::parse_line(
    pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate)
{
    auto directive_start = current;
    while (true) {
        auto a = peek_next();
        if (next().first_in_line)
            break;
    }
    auto directive_end = current;

    auto directive_type = directive_start;
    directive_type++;

    current = directive_type;

    switch (directive_type->type) {
        case pp_token_type::k_define: {
            if (next_in_line().type != pp_token_type::pp_identifier)
                result.errors.emplace_back("Invalid macro name!");

            if (peek_next_in_line().type != pp_token_type::p_leftparentheses) {
                intermediate.obj_macros[current->lexem] = { { current, directive_end } };
            } else {
                parse_func_like(result, intermediate);
            }
            current = directive_end;
        } break;

        case pp_token_type::k_ifdef: {
            if (next_in_line().type != pp_token_type::pp_identifier)
                result.errors.emplace_back("Invalid macro name!");

            std::string macro_name = current->lexem;
            if (!next().first_in_line)
                result.errors.emplace_back("Unexpected tokens at the end of ifdef!");

            conditionals.emplace_back(intermediate.obj_macros.contains(macro_name),
                pp_token_type::k_ifdef, conditionals_depth, current);
            conditionals_depth++;

            skip(result);
        } break;

        case pp_token_type::k_ifndef: {
            if (next_in_line().type != pp_token_type::pp_identifier)
                result.errors.emplace_back("Invalid macro name!");

            std::string macro_name = current->lexem;
            if (!next().first_in_line)
                result.errors.emplace_back("Unexpected tokens at the end of ifndef!");

            conditionals.emplace_back(!intermediate.obj_macros.contains(macro_name),
                pp_token_type::k_ifndef, conditionals_depth, current);
            conditionals_depth++;

            skip(result);
        } break;

        case pp_token_type::k_elifdef: {
            if (next_in_line().type != pp_token_type::pp_identifier)
                result.errors.emplace_back("Invalid macro name!");

            std::string macro_name = current->lexem;
            if (!next().first_in_line)
                result.errors.emplace_back("Unexpected tokens at the end of elifdef!");

            conditionals.emplace_back(intermediate.obj_macros.contains(macro_name),
                pp_token_type::k_ifndef, conditionals_depth, current);

            skip(result);
        } break;

        case pp_token_type::k_elifndef: {
            if (next_in_line().type != pp_token_type::pp_identifier)
                result.errors.emplace_back("Invalid macro name!");

            std::string macro_name = current->lexem;
            if (!next().first_in_line)
                result.errors.emplace_back("Unexpected tokens at the end of elifdef!");

            conditionals.emplace_back(!intermediate.obj_macros.contains(macro_name),
                pp_token_type::k_ifndef, conditionals_depth, current);

            skip(result);
        } break;

        case pp_token_type::k_else: {
            if (!next_in_line().first_in_line)
                result.errors.emplace_back("Unexpected tokens at the end of else!");

            conditionals.emplace_back(true, pp_token_type::k_else, conditionals_depth, current);

            skip(result);
        } break;

        case pp_token_type::k_endif: {
            if (!next_in_line().first_in_line)
                result.errors.emplace_back("Unexpected tokens at the end of endif!");

            conditionals.emplace_back(false, pp_token_type::k_endif, conditionals_depth, current);
            if (conditionals_depth > 0)
                conditionals_depth--;
            else {
                result.errors.emplace_back("Stray endif!");
                break;
            }

            if (conditionals_depth != 0)
                skip(result);
            else
                evaluate_conditionals(result, intermediate);
        } break;

        default:
            result.errors.emplace_back("Unexpected tokens at the end of ifdef!");
            break;
    }

    m_tokens.erase(directive_start, directive_end);
}

void pp_macroprocessor::substitute_func_like_call(
    pp_macroprocessor_intermediate_results &intermediate)
{
    auto &macro = intermediate.func_macros.at(current->lexem);

    auto call_start = current;
    auto prev = current;
    prev--;

    if (next().type != pp_token_type::p_leftparentheses)
        return;

    std::vector<std::pair<std::list<pp_token>::iterator, std::list<pp_token>::iterator>> arg_values;
    std::pair<std::list<pp_token>::iterator, std::list<pp_token>::iterator> variadic_arg;

    auto arg_start = current;
    arg_start++;

    for (; arg_values.size() < macro.arg_count && next_in_line().type != pp_token_type::eol;) {
        auto a = *current;
        if (current->type == pp_token_type::p_comma
            || current->type == pp_token_type::p_rightparentheses) {
            arg_values.emplace_back(arg_start, current);
            arg_start = current;
            arg_start++;
        }

        if (current->type == pp_token_type::p_rightparentheses)
            break;
    }

    if (macro.variadic) {
        if (current->type == pp_token_type::p_rightparentheses) {
            variadic_arg = { current, current };
            return;
        }

        next_in_line();
        auto variadic_start = current;

        for (; next_in_line().type != pp_token_type::eol;) {
            if (current->type == pp_token_type::p_rightparentheses)
                break;
        }

        variadic_arg = { variadic_start, current };
    }

    if (current->type != pp_token_type::p_rightparentheses
        || macro.arg_count != arg_values.size()) {
        next();
        return;
    }

    next();
    auto call_end = current;

    auto substituted = macro.args.begin();
    for (size_t i = 0; i < macro.tokens.size(); i++) {
        if (substituted != macro.args.end() && substituted->position == i) {
            if (substituted->stringify)
                m_tokens.insert(call_start,
                    evaluate_stringify(arg_values[substituted->index].first,
                        arg_values[substituted->index].second));
            else
                m_tokens.insert(call_start, arg_values[substituted->index].first,
                    arg_values[substituted->index].second);
            substituted++;
        } else if (macro.tokens[i].type == pp_token_type::k_va_args) {
            m_tokens.insert(call_start, variadic_arg.first, variadic_arg.second);
        } else
            m_tokens.insert(call_start, macro.tokens[i]);
    }

    m_tokens.erase(call_start, call_end);
    current = prev;
}

void pp_macroprocessor::recursive_substitution(pp_macroprocessor_intermediate_results &intermediate)
{
    for (; !is_end();) {
        if (current->type == pp_token_type::pp_identifier
            && intermediate.obj_macros.contains(current->lexem)) {
            auto &macro_tokens = intermediate.obj_macros.at(current->lexem).tokens;

            auto prev = current;
            prev--;

            m_tokens.insert(current, macro_tokens.begin(), macro_tokens.end());
            m_tokens.erase(current);

            current = prev;
        } else if (current->type == pp_token_type::pp_identifier
            && intermediate.func_macros.contains(current->lexem)) {
            substitute_func_like_call(intermediate);
        } else if (current->type == pp_token_type::p_double_hash) {
            if (!current->first_in_line && !std::next(current)->first_in_line) {
                auto new_token = evaluate_concat(*std::prev(current), *std::next(current));

                if (new_token.has_value()) {
                    auto new_token_iterator
                        = m_tokens.insert(std::prev(current), new_token.value());
                    m_tokens.erase(std::prev(current), std::next(current, 2));

                    current = new_token_iterator;
                }
            }
        }

        if (next().first_in_line || is_end())
            return;
    }
}

void pp_macroprocessor::evaluate_conditionals(
    pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate)
{
    size_t min_evaluate_depth = -1;
    auto begin = current;

    for (size_t i = 0; i < conditionals.size(); i++) {
        auto &current = conditionals[i];

        if (current.type == pp_token_type::k_else) {
            size_t index = i + 1;
            for (; index < conditionals.size(); index++) {
                if (conditionals[index].depth == current.depth)
                    break;
            }

            if (conditionals[index].type == pp_token_type::k_else)
                result.errors.emplace_back("Else after else!");
            else if (conditionals[index].type == pp_token_type::k_elifdef
                || conditionals[index].type == pp_token_type::k_elifndef)
                result.errors.emplace_back("Elif after else!");
        } else if (current.type == pp_token_type::k_endif)
            continue;

        if (min_evaluate_depth < current.depth || !current.included) {
            m_tokens.erase(current.begin, conditionals[i + 1].begin);
        } else {
            if (min_evaluate_depth == current.depth)
                min_evaluate_depth++;

            if (begin == this->current)
                begin = current.begin;
        }
    }

    current = begin;
}

pp_macroprocessor_results pp_macroprocessor::process_tokens(std::vector<pp_token> &tokens)
{
    m_tokens = { tokens.begin(), tokens.end() };
    current = m_tokens.begin();
    conditionals_depth = 0;

    pp_macroprocessor_results result {};
    pp_macroprocessor_intermediate_results intermediate {};

    while (!is_end()) {
        auto a = *current;
        if (!current->first_in_line)
            break;

        if (current->type == pp_token_type::p_hash && !peek_next().first_in_line) {
            parse_line(result, intermediate);
            continue;
        }

        recursive_substitution(intermediate);
    }

    return result;
}

};
