#ifndef PP_MACROPROCESSOR_H
#define PP_MACROPROCESSOR_H

#include "preprocessor/pp_common.h"
#include "preprocessor/pp_token.h"
#include <list>
#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

namespace ctc::preprocessor {

inline static const pp_token eof_token { "", pp_token_type::eof, 0, true };
inline static const pp_token eol_token { "", pp_token_type::eol, 0, true };

struct pp_macroprocessor_results {
    std::vector<pp_token> tokens;
    std::vector<pp_error> errors;
};

struct pp_obj_macro {
    std::vector<pp_token> tokens;
};

struct pp_func_macro_arg {
    size_t position;
    size_t index;

    bool stringify;
};

struct pp_func_macro {
    bool variadic;
    size_t arg_count;
    std::vector<pp_func_macro_arg> args;
    std::vector<pp_token> tokens;
};

struct pp_macroprocessor_intermediate_results {
    std::unordered_map<std::string, pp_obj_macro> obj_macros {};
    std::unordered_map<std::string, pp_func_macro> func_macros {};
};

struct pp_macroprocessor_conditionals {
    bool included;
    pp_token_type type;
    size_t depth;

    std::list<pp_token>::iterator begin;
};

class pp_macroprocessor {
    std::list<pp_token> m_tokens;
    std::list<pp_token>::iterator current;

    std::vector<pp_macroprocessor_conditionals> conditionals;
    size_t conditionals_depth = 0;

    inline const pp_token &next()
    {
        current++;
        if (current == m_tokens.end())
            return eof_token;
        return *current;
    }

    inline const pp_token &next_in_line()
    {
        if (peek_next().first_in_line)
            return eol_token;

        return next();
    }

    inline const pp_token &peek_next() const
    {
        auto temp = current;
        temp++;
        return (temp == m_tokens.end()) ? eof_token : *temp;
    }

    inline const pp_token &peek_next_in_line()
    {
        if (peek_next().first_in_line)
            return eol_token;

        return peek_next();
    }

    bool is_end() { return current == m_tokens.end(); }

    pp_token evaluate_stringify(
        std::list<pp_token>::iterator first, std::list<pp_token>::iterator last);
    std::optional<pp_token> evaluate_concat(pp_token &first, pp_token &second);

    void substitute_func_like_call(pp_macroprocessor_intermediate_results &intermediate);
    void recursive_substitution(pp_macroprocessor_intermediate_results &intermediate);

    void parse_func_like(
        pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate);
    void parse_line(
        pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate);

    void skip(pp_macroprocessor_results &result);
    void evaluate_conditionals(
        pp_macroprocessor_results &result, pp_macroprocessor_intermediate_results &intermediate);

public:
    pp_macroprocessor_results process_tokens(std::vector<pp_token> &tokens);
};

};

#endif // PP_MACROPROCESSOR_H
