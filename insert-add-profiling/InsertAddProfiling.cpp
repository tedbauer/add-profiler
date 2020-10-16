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
      errs() << "Hello!\n";
      LLVMContext& Ctx = F.getContext();
      FunctionCallee logFunc = F.getParent()->getOrInsertFunction(
        "logop", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx)
      );

      errs() << F << "\n";
      for (auto &B : F) {
	errs() << B << "\n";
        for (auto &I : B) {
	  errs() << I << "\n";
          if (auto *op = dyn_cast<BinaryOperator>(&I)) {
            IRBuilder<> builder(op);
            builder.SetInsertPoint(&B, ++builder.GetInsertPoint());

	    Value* args[] = {op};
	    builder.CreateCall(logFunc, args);

	    return true;
          }
	}
      }

      return false;
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
