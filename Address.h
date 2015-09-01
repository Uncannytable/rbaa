//===--- Address.h - Pass definition ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the declaration of the Address class for the
/// Ranged Based Alias Analysis
///
//===----------------------------------------------------------------------===//
#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include <set>
#include <map>
#include <deque>

namespace llvm {

/// Forward declarations
class RangedPointer;
//class Range;
//class Expr;
class NarrowingOp;
class Value;


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
  
  bool argument;
  
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

}

#endif
