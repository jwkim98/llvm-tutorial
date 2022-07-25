#include <cpp-oss-template/Test.hpp>
#include <cpp-oss-template/lexer.hpp>
#include <cpp-oss-template/semantics.hpp>
#include <cpp-oss-template/parser.hpp>

int Add(int a, int b)
{
    return a + b;
}

bool Compile()
{
    char buf[] = "with a,b,c : (3 + 4 + a)";
    Lexer lexer(buf);
    Parser parser(lexer);
    auto* ast = parser.parse();
    Sema semantics;
    auto res = semantics.semantic(ast);
    return res;
}