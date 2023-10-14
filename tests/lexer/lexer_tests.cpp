#include <lexer.h>

#include <cstdio>

#define SIMPLE_TEST_SOURCE ("// Is Comment,\n +-=/--==--=-=<<>>>>>>")

int main()
{
    puts("Initializing lexer....");

    ctc::lexer::lexer_options options {false};
    ctc::lexer::lexer lexer(options);

    printf("Test 1, source: %s", SIMPLE_TEST_SOURCE);
    auto tokens_from_test_1 = lexer.tokenize_from_source(SIMPLE_TEST_SOURCE).tokens;

    for (auto& token : tokens_from_test_1)
    {
        printf("\nToken Type: %s, Lexeme: %s\n", token.get_text().c_str(), token.lexeme.c_str());
    }
}
