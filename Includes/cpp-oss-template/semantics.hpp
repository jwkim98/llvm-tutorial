#ifndef SEMA_H
#define SEMA_H
#include <cpp-oss-template/AST.hpp>
#include <llvm/ADT/StringSet.h>
#include <llvm/Support/raw_ostream.h>

class Sema
{
public:
    bool semantic(AST* tree);
};

class DeclCheck : public ASTVisitor
{
    llvm::StringSet<> Scope;
    bool HasError;

    enum ErrorType
    {
        Twice,
        Not
    };

    void error(ErrorType erorType, llvm::StringRef variable);

    void visit(Factor& node) override;
    void visit(BinaryOp& node) override;

    void visit(WithDecl& node) override;

public:
    DeclCheck()
        : HasError(false)
    {
    }

    bool hasError()
    {
        return HasError;
    }
};


#endif
