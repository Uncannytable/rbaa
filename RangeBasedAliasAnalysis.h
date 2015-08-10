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
  
  void getAnalysisUsage(AnalysisUsage &AU) const override;
  bool runOnFunction(Function &F) override;
  AliasResult alias(const Location &LocA, const Location &LocB) override;

  


  ModRefResult getModRefInfo(ImmutableCallSite CS, const Location &Loc) override;

  ModRefResult getModRefInfo(ImmutableCallSite CS1,
                             ImmutableCallSite CS2) override {
    // The AliasAnalysis base class has some smarts, lets use them.
    return AliasAnalysis::getModRefInfo(CS1, CS2);
  }

  /// pointsToConstantMemory - Chase pointers until we find a (constant
  /// global) or not.
  bool pointsToConstantMemory(const Location &Loc, bool OrLocal) override;

  /// Get the location associated with a pointer argument of a callsite.
  Location getArgLocation(ImmutableCallSite CS, unsigned ArgIdx,
                          ModRefResult &Mask) override;

  /// getModRefBehavior - Return the behavior when calling the given
  /// call site.
  ModRefBehavior getModRefBehavior(ImmutableCallSite CS) override;

  /// getModRefBehavior - Return the behavior when calling the given function.
  /// For use when the call site is not known.
  ModRefBehavior getModRefBehavior(const Function *F) override;
   
};

}


#endif
