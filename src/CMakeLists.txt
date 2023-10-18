add_library(src ${LIB_TYPE}
    lexer/lexer.h
    lexer/token.h

    preprocessor/preprocessor.h
    preprocessor/pp_token.h
    preprocessor/pp_lexer.h
    preprocessor/pp_macroprocessor.h

    ast/ast.h
    ast/ast_visitor.h

    lexer/token.cpp
    lexer/lexer.cpp

    preprocessor/pp_lexer.cpp
    preprocessor/pp_macroprocessor.cpp

    ast/ast.cpp
    ast/ast_visitor.cpp
)
