#define DEBUG_TYPE "range-based-aa"
#include "RangeBasedAliasAnalysis.h"
using namespace llvm;

extern SAGEInterface *SI;

// Register this pass...
char RangeBasedAliasAnalysis::ID = 0;
static RegisterPass<RangeBasedAliasAnalysis> X("range-based-aa",
"Range Analysis based Alias Analysis", false, false);
static RegisterAnalysisGroup<AliasAnalysis> E(X);

void RangeBasedAliasAnalysis::getAnalysisUsage(AnalysisUsage &AU) const 
{
  AliasAnalysis::getAnalysisUsage(AU);
  AU.addRequired<RangeBasedPointerAnalysis>();
  AU.setPreservesAll();
}

bool RangeBasedAliasAnalysis::runOnFunction(Function &F) 
{
  InitializeAliasAnalysis(this);
  rbpa = &(getAnalysis<RangeBasedPointerAnalysis>());
  return false;
}

Expr higher(Expr e1, Expr e2)
{
  if(e1.isConstant() && e2.isConstant() )
    return (e1 > e2);
  else 
    return Expr::getFalse(*SI);
}
Expr lower(Expr e1, Expr e2)
{
  if(e1.isConstant() && e2.isConstant() )
    return (e1 < e2);
  else 
    return Expr::getFalse(*SI);
}

bool RangeBasedAliasAnalysis::eval(RangedPointer* rp1, RangedPointer* rp2)
{
  for(auto i = rp1->addr_begin(), ie = rp1->addr_end(); i != ie; i++)
  {
    Address* ai = *i;
    for(auto j = rp2->addr_begin(), je = rp2->addr_end(); j != je; j++)
    {
      Address* aj = *j;
      
      /*errs() << "Compare: ";
      ai->print();
      errs() << " X ";
      aj->print();
      errs() << "\n";*/
      
      bool disjoint = false;
      if( ai->getBase()->getPointerType() == RangedPointer::Alloc
      and aj->getBase()->getPointerType() == RangedPointer::Alloc
      and ai->getBase() != aj->getBase())
      {
        disjoint = true;
      }
      else if(ai->getBase() == aj->getBase())
      {
        Expr ex = 
          //higher(ai->getOffset()->getLower(), aj->getOffset()->getUpper()) || 
          //lower(ai->getOffset()->getUpper(), aj->getOffset()->getLower());
          ai->getOffset()->getLower() > aj->getOffset()->getUpper() ||
          ai->getOffset()->getUpper() < aj->getOffset()->getLower();
        if(ex.isEQ(Expr::getTrue(*SI)))
          disjoint = true;
      }
      
      if(!disjoint)
        return false;
    }
  }
  return true;
}

RangeBasedAliasAnalysis::AliasResult 
RangeBasedAliasAnalysis::alias(const Location &LocA, const Location &LocB)
{
  const Value *p1, *p2;
  p1 = LocA.Ptr;
  p2 = LocB.Ptr;
  
  RangedPointer* rp1 = rbpa->getRangedPointer(p1);
  RangedPointer* rp2 = rbpa->getRangedPointer(p2);
  
  //errs() << *(rp1->getPointer()) << "\n";
  //errs() << *(rp2->getPointer()) << "\n";
  
  /// Phase 1 evaluation
  
  if(rp1->phase1_base == rp2->phase1_base)
  {
    Expr ex = 
      //higher(rp1->phase1_offset->getLower(), rp2->phase1_offset->getUpper()) ||
      //lower(rp1->phase1_offset->getUpper(), rp2->phase1_offset->getLower());
      rp1->phase1_offset->getLower() > rp2->phase1_offset->getUpper() ||
      rp1->phase1_offset->getUpper() < rp2->phase1_offset->getLower();
    if(ex.isEQ(Expr::getTrue(*SI)))
    {
      //errs() << "-----------NoAlias------------\n";
      return NoAlias;
    }
  }
  
  /// Phase 2 evaluation
  
  if(eval(rp1,rp2))
  {
    //errs() << "-----------NoAlias------------\n";
    return NoAlias;
  }  
  
  
  //errs() << "-----------MayAlias-------------\n";
    
  return AliasAnalysis::alias(LocA, LocB);
}
