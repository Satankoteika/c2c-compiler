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

    void make_macro(size_t index, std::unordered_map<std::string, pp_macro> &macros);

public:
    pp_macroprocessor_results process_tokens(std::vector<pp_token> &&tokens);
};

};

#endif // PP_MACROPROCESSOR_H
