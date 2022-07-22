#include <cpp-oss-template/lexer.hpp>


namespace charinfo
{
LLVM_READNONE inline bool isWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\f' || c == '\v' || c == '\r' ||
           c == '\n';
}

LLVM_READNONE inline bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

LLVM_READNONE inline bool isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
}

llvm::StringRef Token::getText() const
{
    return m_text;
}


void Lexer::next(Token& token)
{
    while (*m_bufferPtr && charinfo::isWhitespace(*m_bufferPtr))
    {
        ++m_bufferPtr;
    }

    if (!*m_bufferPtr)
    {
        token.m_kind = Token::eoi;
        return;
    }

    if (charinfo::isLetter(*m_bufferPtr))
    {
        const char* end = m_bufferPtr + 1;
        while (charinfo::isLetter(*end))
            ++end;
        llvm::StringRef name(m_bufferPtr, end - m_bufferPtr);
        Token::TokenKind kind = name == "with" ? Token::KW_with : Token::ident;
        formToken(token, end, kind);
    }
    else if (charinfo::isDigit(*m_bufferPtr))
    {
        const char* end = m_bufferPtr + 1;
        while (charinfo::isDigit(*end))
            ++end;
        llvm::StringRef name(m_bufferPtr, end - m_bufferPtr);
        formToken(token, end, Token::number);
    }
    else
    {
#define CASE(ch, tok)                         \
    case ch:                                  \
        formToken(token, m_bufferPtr + 1, tok); \
        break

        switch (*m_bufferPtr)
        {
            CASE('+', Token::TokenKind::plus);
            CASE('-', Token::TokenKind::minus);
            CASE('*', Token::TokenKind::star);
            CASE('/', Token::TokenKind::slash);
            CASE('(', Token::TokenKind::l_parsen);
            CASE(')', Token::TokenKind::r_parsen);
            CASE(':', Token::TokenKind::colon);
            CASE(',', Token::Token::TokenKind::comma);
            default:
                formToken(token, m_bufferPtr + 1, Token::TokenKind::unknown);
        }
    }
}

void Lexer::formToken(Token& result, const char* toEnd, Token::TokenKind kind)
{
    result.m_kind = kind;
    result.m_text = llvm::StringRef(m_bufferPtr, toEnd - m_bufferPtr);
    m_bufferPtr = toEnd;
}
