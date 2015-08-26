//===--- RangedPointer.h - Pass definition ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the declaration of the RangedPointer class for the
/// Ranged Based Alias Analysis
///
//===----------------------------------------------------------------------===//
#ifndef __RANGED_POINTER_H__
#define __RANGED_POINTER_H__

#include <set>

namespace llvm {

/// Forward declarations
class Value;
class Address;
//class Range;
class Value;
class Use;
class RangeBasedPointerAnalysis;

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

}

#endif
