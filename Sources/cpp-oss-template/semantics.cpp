#include <cpp-oss-template/semantics.hpp>

bool Sema::semantic(AST* tree)
{
    if (!tree)
        return false;
    DeclCheck check;
    tree->accept(check);
    return check.hasError();
}


void DeclCheck::error(ErrorType erorType, llvm::StringRef variable)
{
    llvm::errs() << "Variable " << variable << " "
        << (erorType == Twice ? "already" : "not") << " declared\n";
}

void DeclCheck::visit(Factor& node)
{
    if (node.getKind() == Factor::Ident)
    {
        if (Scope.find(node.getVal()) == Scope.end())
            error(ErrorType::Not, node.getVal());
    }
}

void DeclCheck::visit(BinaryOp& node)
{
    const auto left = node.getLeft();
    const auto right = node.getRight();

    if (left)
        left->accept(*this);
    else
        HasError = true;

    if (right)
        right->accept(*this);
    else
        HasError = true;
}

void DeclCheck::visit(WithDecl& node)
{
    for (auto itr = node.begin(); itr != node.end(); ++itr)
    {
        if (!Scope.insert(*itr).second)
            error(ErrorType::Twice, *itr);
    }

    if (node.getExpr())
        node.getExpr()->accept(*this);
    else
        HasError = true;
}
