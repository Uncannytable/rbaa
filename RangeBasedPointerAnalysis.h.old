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

/// Representation of types as sequences of primitive values
class Primitives
{
  public:
  //Holds a Primitive Layout for a determined Type
  struct PrimitiveLayout
  {
    Type * type;
    std::vector<int> layout;
    PrimitiveLayout(Type* ty, std::vector<int> lay)
    {
      type = ty;
      layout = lay;
    }
  };
  struct NumPrimitive
  {
    Type * type;
    int num;
    NumPrimitive(Type* ty, int n)
    {
      type = ty;
      num = n;
    }
  };
  std::vector<PrimitiveLayout*> PrimitiveLayouts;
  std::vector<NumPrimitive*> NumPrimitives;
  std::vector<int> getPrimitiveLayout(Type* type);
  int getNumPrimitives(Type* type);
  llvm::Type* getTypeInside(Type* type, int i);
  int getSumBehind(std::vector<int> v, unsigned int i);
};

/// Representation of a possible pointer address. It is composed,
/// essencially, of a base pointer and an offset
class Address
{
  public:
  RangedPointer* Base;
  RangedPointer* Addressee;
  Range* Offset;
  bool widened;
  std::map<RangedPointer*, Range> expanded;
  std::map<const Value*, NarrowingOp*> Narrowing_Ops;
  
  Address(RangedPointer* addressee, RangedPointer* base, Range* offset);
  Address(Address*);
  ~Address();
  RangedPointer* getBase();
  RangedPointer* getAddressee();
  void setBase(RangedPointer*);
  void setOffset(Expr lower, Expr upper);
  Range* getOffset();
  void print();
  void Expand (std::deque<Address*> &ad, std::set<Address*> &fn);
};

/// \brief Pointer representation in which each pointer is a set of
/// possible addresses
class RangedPointer
{
  friend class Address;
  public:
  enum PointerTypes
  {
    Unk = 0,
    Alloc = 1,
    Phi = 2,
    Cont = 3,
    Null = 4
  };
    
  const Value* Pointer;
  std::set<Address*> Addresses;
  std::set<Address*> Bases;
  PointerTypes PointerType;
  int color;
  int scc;
  RangedPointer* phase1_base;
  Range* phase1_offset;
  
  RangedPointer(const Value* pointer);
  RangedPointer(const Value* pointer, PointerTypes pointer_type);
  RangedPointer(RangedPointer*);
  void processGEP(RangedPointer*, const Use*, const Use*);
  void processInitialAddresses(RangeBasedPointerAnalysis* analysis);
  const Value* getPointer();
  void setPointerType(PointerTypes);
  enum PointerTypes getPointerType();
  std::set<Address*>::iterator addr_begin();
  std::set<Address*>::iterator addr_end();
  bool addr_empty();
  std::set<Address*>::iterator bases_begin();
  std::set<Address*>::iterator bases_end();
  void print();
  bool isEvil();
};

/// classes that help implementing the narrowing operations
class NarrowingData
{
  public:
  CmpInst::Predicate cmp_op;
  Value* cmp_v1;
  Value* cmp_v2;
  std::set<const PHINode*> sigmas;
};

class NarrowingOp
{
  public:
  CmpInst::Predicate cmp_op;
  Value* cmp_v;
  Range* context;
  
  NarrowingOp* contextualize(Range* c);
  ~NarrowingOp();
};

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
  
};

}

#endif
