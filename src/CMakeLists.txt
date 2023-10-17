add_library(src ${LIB_TYPE}
    lexer/lexer.h
    lexer/token.h

    preprocessor/pp_token.h
    preprocessor/pp_lexer.h
    preprocessor/preprocessor.h
    preprocessor/pp_macroprocessor.h

    lexer/token.cpp
    lexer/lexer.cpp

    preprocessor/pp_lexer.cpp
<<<<<<< HEAD

    ast/ast.h
    ast/ast.cpp

    ast/ast_visitor.h
    ast/ast_visitor.cpp
=======
    preprocessor/pp_macroprocessor.cpp
>>>>>>> cd7af93 (Unit tests added, macoprocessor started)
)

<<<<<<< HEAD
include_directories(./ lexer/)
=======
>>>>>>> e709dab (Preprocessor lexer completed, simple test added, eeds automatition)
