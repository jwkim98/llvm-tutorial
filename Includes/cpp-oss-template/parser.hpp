#ifndef PARSER_HPP
#define PARSER_HPP

#include <cpp-oss-template/AST.hpp>
#include <cpp-oss-template/lexer.hpp>
#include <llvm/Support/raw_ostream.h>

class Parser
{
public:
    Parser(Lexer& lexer)
        : m_lexer(lexer)
    {
        advance();
    }

    //! Retrieves the next token if the look-ahead is one of expected kind
    bool consume(Token::TokenKind kind)
    {
        if (expect(kind))
            return true;
        advance();
        return false;
    }

    AST* parse();
    AST* parseCalc();
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();

private:
    //! retrieves the next token from the lexer
    //! and saves it to m_token
    void advance()
    {
        m_lexer.next(m_token);
    }

    //! tests whether look-ahead is the expected kind and emits an error message
    //! if not
    bool expect(Token::TokenKind kind)
    {
        if (m_token.getKind() != kind)
        {
            error();
            return false;
        }
        return true;
    }

    //! Emits error
    void error()
    {
        llvm::errs() << "Unexpected: " << m_token.getText() << "\n";
        m_hasError = true;
    }

    //! Used to to retrieve the next token 
    Lexer& m_lexer;
    //! Stores the next token to read
    Token m_token;
    bool m_hasError = false;
};

#endif
