#ifndef PP_COMMON_H
#define PP_COMMON_H

#include <string>

namespace ctc::preprocessor {

struct pp_error {
    std::string what;
    size_t line, pos;
};

struct pp_options {
    bool expand_mode;
};

};

#endif // PP_COMMON_H
