#ifndef LEXER_HPP
#define LEXER_HPP

#include <llvm/ADT/StringRef.h>
//! MemoryBuffer provides read only access to a block of memory, filled with the content of a file.
#include <llvm/Support/MemoryBuffer.h>

class Lexer;

class Token
{
    friend class Lexer;
public:
    Token() = default;
    enum TokenKind : unsigned short
    {
        eoi,
        unknown,
        ident,
        number,
        comma,
        colon,
        plus,
        minus,
        star,
        slash,
        l_parsen,
        r_parsen,
        KW_with
    };

    [[nodiscard]] TokenKind getKind() const
    {
        return m_kind;
    }

    [[nodiscard]] llvm::StringRef getText() const;

    //! is and isOneOf methods are used to check if token is of a certain kind
    [[nodiscard]] bool is(TokenKind kind) const
    {
        return m_kind == kind;
    }

    [[nodiscard]] bool isOneOf(TokenKind kind1, TokenKind kind2) const
    {
        return is(kind1) || is(kind2);
    }

    template <typename... Ts>
    [[nodiscard]] bool isOneOf(TokenKind kind1, TokenKind kind2, Ts ... tokens) const
    {
        return is(kind1) || isOneOf(kind2, tokens...);
    }


private:
    //! Indicates the kind of the token
    TokenKind m_kind = TokenKind::unknown;
    //! Points to the start of the text of the token
    llvm::StringRef m_text;
};

class Lexer
{
public:
    Lexer(const llvm::StringRef& buffer)
    {
        m_bufferStart = buffer.begin();
    }

    void next(Token& token);

private :
    void formToken(Token& result, const char* toEnd, Token::TokenKind kind);

    const char* m_bufferStart;
    const char* m_bufferPtr;
};

#endif
