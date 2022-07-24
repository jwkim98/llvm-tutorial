#include <cpp-oss-template/sema.hpp>
#include <llvm/ADT/StringSet.h>
#include <llvm/Support/raw_ostream.h>

class DeclCheck : public ASTVisitor
{
    llvm::StringSet<> Scope;
    bool HasError;

    enum ErrorType { Twice, Not };

    void error(ErrorType erorType, llvm::StringRef variable)
    {
        llvm::errs()
            << "Variable " << variable << " "
            << (erorType == Twice ? "already" : "not") << " declared\n";
    }

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
