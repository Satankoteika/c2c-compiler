add_library(src ${LIB_TYPE}
    lexer/lexer.h
    lexer/token.h

    preprocessor/pp_token.h
    preprocessor/pp_lexer.h
    preprocessor/preprocessor.h

    lexer/token.cpp
    lexer/lexer.cpp

    preprocessor/pp_lexer.cpp

    ast/ast.h
    ast/ast.cpp

    ast/ast_visitor.h
    ast/ast_visitor.cpp
)

include_directories(./ lexer/)
