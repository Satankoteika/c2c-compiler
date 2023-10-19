#include "preprocessor/pp_lexer.h"
#include "preprocessor/pp_macroprocessor.h"

int main()
{
    ctc::preprocessor::pp_macroprocessor proc {};
    ctc::preprocessor::pp_lexer lex { {} };
    // auto tokens = lex.tokenize_from_source("#define MACRO 123").tokens;
    proc.process_tokens(lex.tokenize_from_source("#define MACRO 123 456\n print Macro").tokens);
}
