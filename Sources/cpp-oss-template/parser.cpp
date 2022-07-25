#include <cpp-oss-template/parser.hpp>

AST* Parser::parse()
{
    AST* result = parseCalc();
    expect(Token::eoi);
    return result;
}


//! calc : ("with" ident ( "," ident)* ":")? expr;
AST* Parser::parseCalc()
{
    llvm::SmallVector<llvm::StringRef, 8> vars;
    if (m_token.is(Token::KW_with))
    {
        advance();

        if (!expect(Token::ident))
            error();

        vars.push_back(m_token.getText());
        advance();

        while (m_token.is(Token::comma))
        {
            advance();
            if (!expect(Token::ident))
                error();

            vars.push_back(m_token.getText());
            advance();
        }
        if (!consume(Token::colon))
            error();
    }

    Expr* expr = parseExpr();
    if (vars.empty())
        return expr;
    return new WithDecl(vars, expr);
}

//! expr : term (( "+" | "-" _ term)*
Expr* Parser::parseExpr()
{
    Expr* base = parseTerm();
    while (m_token.isOneOf(Token::plus, Token::minus))
    {
        BinaryOp::Operator op =
            m_token.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minus;
        advance();
        Expr* right = parseTerm();
        base = new BinaryOp(op, base, right);
    }

    return base;
}

//! term : factor (("*"|"/") factor)*
Expr* Parser::parseTerm()
{
    Expr* base = parseFactor();
    while (m_token.isOneOf(Token::star, Token::slash))
    {
        BinaryOp::Operator op =
            m_token.is(Token::star) ? BinaryOp::Mul : BinaryOp::Div;
        advance();
        Expr* right = parseFactor();
        base = new BinaryOp(op, base, right);
    }
    return base;
}

//! factor : ident | number  "(" expr ")"
Expr* Parser::parseFactor()
{
    Expr* res = nullptr;
    switch (m_token.getKind())
    {
        case Token::number:
            res = new Factor(Factor::Number, m_token.getText());
            advance();
            break;
        case Token::ident:
            res = new Factor(Factor::Ident, m_token.getText());
            advance();
            break;
        case Token::l_parsen:
            advance();
            res = parseExpr();
            if (consume(Token::r_parsen))
                break;
        default:
            if (!res)
                error();
            while (!m_token.isOneOf(Token::r_parsen, Token::star, Token::plus,
                                    Token::minus, Token::slash, Token::eoi))
                advance();
    }
    return res;
}
