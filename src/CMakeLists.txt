add_library(src ${LIB_TYPE}
    lexer/lexer.h
    lexer/token.h

    lexer/token.cpp
    lexer/lexer.cpp
)

include_directories(./)
