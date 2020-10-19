#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"
using namespace llvm;


namespace {
  struct InsertAddProfilingPass : public FunctionPass {
    static char ID;
    InsertAddProfilingPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      LLVMContext& Ctx = F.getContext();

      FunctionCallee logNumberFunc = F.getParent()->getOrInsertFunction(
        "log_result", Type::getVoidTy(Ctx), Type::getVoidTy(Ctx)
      );

      auto &firstBlock = F.front();
      auto &firstInstr = firstBlock.front();

      IRBuilder<> builder(&firstInstr);
      auto allocInst = builder.CreateAlloca(Type::getInt32Ty(Ctx), 0, "");
      
      FunctionCallee logResultFunc = F.getParent()->getOrInsertFunction(
        "log_result", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx)
      );

      Value* incremented;

      for (auto &B : F) {
        for (auto &I : B) {
          if (isa<BinaryOperator>(I) && I.getOpcode() == Instruction::Mul) {
            auto *op = dyn_cast<BinaryOperator>(&I);
            IRBuilder<> builder2(op);
	    builder2.SetInsertPoint(&B, ++builder2.GetInsertPoint());
	    LoadInst *loadInst = builder2.CreateLoad(Type::getInt32Ty(Ctx), allocInst);
	    incremented = builder2.CreateAdd(builder2.getInt32(1), loadInst);
	    StoreInst *storeInst = builder2.CreateStore(incremented, allocInst);
          }
	}
      }

      auto &lastBlock = F.back();
      auto &lastInstr = lastBlock.back();

      Value *funcName = builder.CreateGlobalStringPtr(F.getName());
      std::vector<Value*> args({funcName, incremented});
      IRBuilder<> builder3(&lastInstr);
      builder3.CreateCall(logResultFunc, args);

      return true;
    }

  };
}

char InsertAddProfilingPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerInsertAddProfilingPass(const PassManagerBuilder &,
                                           legacy::PassManagerBase &PM) {
  PM.add(new InsertAddProfilingPass());
}

static RegisterStandardPasses RegisterMyPass(
  PassManagerBuilder::EP_EarlyAsPossible,
  registerInsertAddProfilingPass
);
