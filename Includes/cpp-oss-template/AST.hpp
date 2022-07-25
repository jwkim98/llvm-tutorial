#ifndef AST_HPP
#define AST_HPP

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

class AST;
class Expr;
class Factor;
class BinaryOp;
class WithDecl;

//! Class for using visitor pattern
class ASTVisitor
{
public:
    virtual void visit(AST&)
    {
    };

    virtual void visit(Expr&)
    {
    };

    virtual void visit(Factor&) = 0;
    virtual void visit(BinaryOp&) = 0;
    virtual void visit(WithDecl&) = 0;
};

//! Root of the hierarchy
class AST
{
public:
    virtual ~AST()
    {
    }

    virtual void accept(ASTVisitor& v) = 0;
};

//! Root for the AST class
class Expr : public AST
{
public:
    Expr()
    {
    }
};

//! Stores a number or the name of a variable
class Factor : public Expr
{
public:
    enum ValueKind { Ident, Number };

    Factor(ValueKind kind, llvm::StringRef val)
        : m_kind(kind),
          m_val(val)
    {
    }

    ValueKind getKind()
    {
        return m_kind;
    }

    llvm::StringRef getVal()
    {
        return m_val;
    }

    void accept(ASTVisitor& value) override
    {
        value.visit(*this);
    }

private:
    ValueKind m_kind;
    llvm::StringRef m_val;
};

//! Represents binary operation
class BinaryOp : public Expr
{
public:
    enum Operator { Plus, Minus, Mul, Div };

    BinaryOp(Operator op, Expr* left, Expr* right)
        : m_op(op),
          m_left(left),
          m_right(right)
    {
    }

    [[nodiscard]] Expr* getLeft() const
    {
        return m_left;
    }

    [[nodiscard]] Expr* getRight() const
    {
        return m_right;
    }

    [[nodiscard]] Operator getOperator() const
    {
        return m_op;
    }

    void accept(ASTVisitor& visitor) override
    {
        visitor.visit(*this);
    }

private:
    Operator m_op;
    Expr* m_left;
    Expr* m_right;
};

//! Stores declared variables and the expression
class WithDecl : public AST
{
    using VarVector = llvm::SmallVector<llvm::StringRef, 8>;
    VarVector m_vars;

    Expr* m_expr;
public:
    WithDecl(llvm::SmallVector<llvm::StringRef, 8> vars, Expr* expr)
        : m_vars(std::move(vars)),
          m_expr(expr)
    {
    }

    VarVector::const_iterator begin()
    {
        return m_vars.begin();
    }

    VarVector::const_iterator end()
    {
        return m_vars.end();
    }

    [[nodiscard]] Expr* getExpr() const
    {
        return m_expr;
    }

    void accept(ASTVisitor& value) override
    {
        value.visit(*this);
    }
};

#endif
