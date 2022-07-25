#include <cpp-oss-template/CodeGen.hpp>
#include <llvm/ADT/StringMap.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/support/raw_ostream.h>


class ToIRVisitor : public ASTVisitor
{
public:
    ToIRVisitor(llvm::Module* module)
        : m_module(module),
          m_builder(module->getContext())
    {
        m_voidType = llvm::Type::getVoidTy(module->getContext());
        m_int32Type = llvm::Type::getInt32Ty(module->getContext());
        m_int8PtrType = llvm::Type::getInt8PtrTy(m_module->getContext());
        m_int8PtrPtrType = m_int8PtrType->getPointerTo();
        m_int32Zero = llvm::ConstantInt::get(m_int32Type, 0, true);
    }

    void run(AST* tree)
    {
        llvm::FunctionType* mainFunctionType = llvm::FunctionType::get(
            m_int32Type, { m_int32Type, m_int8PtrPtrType }, false);
        llvm::Function* mainFunction = llvm::Function::Create(
            mainFunctionType, llvm::GlobalValue::ExternalLinkage, "main",
            m_module);
        llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(
            m_module->getContext(), "entry", mainFunction);
        m_builder.SetInsertPoint(basicBlock);

        tree->accept(*this);

        llvm::FunctionType* calcWriteFunctionType =
            llvm::FunctionType::get(m_voidType,
                                    { m_int32Type }, false);
        llvm::Function* calcWriteFunction = llvm::Function::Create(
            calcWriteFunctionType, llvm::GlobalValue::ExternalLinkage,
            "calc_write", m_module);
        m_builder.CreateCall(calcWriteFunctionType, calcWriteFunction,
                             { m_value });
        m_builder.CreateRet(m_int32Zero);
    }

    void visit(WithDecl& node) override
    {
        llvm::FunctionType* readFunctionType =
            llvm::FunctionType::get(m_int32Type, { m_int8PtrType }, false);
        llvm::Function* readFunction = llvm::Function::Create(
            readFunctionType, llvm::GlobalValue::ExternalLinkage, "calc_read",
            m_module);

        for (auto itr = node.begin(); itr != node.end(); ++itr)
        {
            llvm::StringRef var = *itr;
            llvm::Constant* strText = llvm::ConstantDataArray::getString(
                m_module->getContext(), var);
            auto* str = new llvm::GlobalVariable(
                *m_module, strText->getType(), /*isConstant*/ true,
                llvm::GlobalValue::PrivateLinkage, strText,
                llvm::Twine(var).concat(".str"));

            llvm::Value* ptr = m_builder.CreateInBoundsGEP(
                str, { m_int32Zero, m_int32Zero }, "ptr");
            llvm::CallInst* call =
                m_builder.CreateCall(readFunctionType, readFunction, { ptr });
        }
    }

private:
    llvm::Module* m_module;
    llvm::IRBuilder<> m_builder;
    llvm::Type* m_voidType;
    llvm::Type* m_int32Type;
    llvm::Type* m_int8PtrType;
    llvm::Type* m_int8PtrPtrType;
    llvm::Constant* m_int32Zero;
    llvm::Value* m_value;
    llvm::StringMap<llvm::Value*> m_nameMap;
};
