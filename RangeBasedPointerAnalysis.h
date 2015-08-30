//===--- RangeBasedPointerAnalysis.h - Pass definition ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the declaration of the Range Based Pointer Analy-
/// sis pass. It's used as a base pass for it's respective alias analysis
///
//===----------------------------------------------------------------------===//
#ifndef __RANGE_BASED_POINTER_ANALYSIS_H__
#define __RANGE_BASED_POINTER_ANALYSIS_H__

#include "../llvm-sra/SymbolicRangeAnalysis.h"

#include "llvm/Pass.h"

#include <set>
#include <map>
#include <vector>
#include <deque>

typedef SymbolicRangeAnalysis RangeAnalysis;
typedef SAGEExpr Expr;
typedef SAGERange Range;

namespace llvm {

/// Forward declarations
class Value;
class Use;
class Type;
class RangedPointer;
class RangeBasedPointerAnalysis;
class NarrowingOp;
class NarrowingData;

/// LLVM passes that run the new pointer representation
class RangeBasedPointerAnalysis: public ModulePass
{
  public:
  std::map<const Value*, RangedPointer*> RangedPointers;
  std::map<Value*, NarrowingData*> narrowing_data;
  
  void printDOT(Module &M, std::string stage);
  void DFS_visit(RangedPointer*, std::deque<RangedPointer*>*);
  void Phase1_DFS_visit(RangedPointer*, std::deque<RangedPointer*>*);
  void Phase2_DFS_visit1(RangedPointer*, std::deque<RangedPointer*>*);
  void Phase2_DFS_visit2(RangedPointer*, int, int&);
  void SCC_DFS_visit(RangedPointer*, std::deque<RangedPointer*>*);
  void PreNarrowing();
  RangedPointer* getRangedPointer(const Value*);
  
  /// LLVM framework methods and atributes
  static char ID;
  RangeBasedPointerAnalysis() : ModulePass(ID){}
  bool runOnModule(Module &M);
  void getAnalysisUsage(AnalysisUsage &AU) const;
  
  std::set<const Value*> AllPointers;
  std::set<const StoreInst*> RelevantStores;
  
  void gatherPointers(Module &M, int &ninst);
  void buildDependenceGraph(int &npointers);
  void loadStoreConnect();
  std::map<int,std::pair<RangedPointer*, int> > findSCCs();
  void resolveSCCs(std::map<int,std::pair<RangedPointer*, int> > sccs);
  void resolveWholeGraph();
  void applyNarrowing();
  
};

}

#endif
