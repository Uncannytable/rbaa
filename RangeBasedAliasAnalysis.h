#ifndef __RANGE_BASED_ALIAS_ANALYSIS_H__
#define __RANGE_BASED_ALIAS_ANALYSIS_H__
#include "RangeBasedPointerAnalysis.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Pass.h"

namespace llvm
{

class RangeBasedAliasAnalysis : public FunctionPass, public AliasAnalysis
{

  public:
  static char ID; // Class identification, replacement for typeinfo
  RangeBasedAliasAnalysis() : FunctionPass(ID){}

	/// getAdjustedAnalysisPointer - This method is used when a pass implements
  /// an analysis interface through multiple inheritance.  If needed, it
  /// should override this to adjust the this pointer as needed for the
  /// specified pass info.
  virtual void *getAdjustedAnalysisPointer(AnalysisID PI) {
    if (PI == &AliasAnalysis::ID)
      return (AliasAnalysis*)this;
    return this;
  }

  private:
  RangeBasedPointerAnalysis* rbpa;
  
  bool eval(RangedPointer* rp1, RangedPointer* rp2);
  
  virtual void getAnalysisUsage(AnalysisUsage &AU) const;
  virtual bool runOnFunction(Function &F);
  virtual AliasResult alias(const Location &LocA, const Location &LocB);
};

}


#endif
