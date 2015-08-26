//===--- Narrowing.h - Pass definition ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the declaration of narrowing classes for the
/// Ranged Based Alias Analysis
///
//===----------------------------------------------------------------------===//
#ifndef __NARROWING_H__
#define __NARROWING_H__

#include "llvm/Pass.h"

#include <set>

namespace llvm {

/// Forward declarations
class Value;
//class Range;

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

}

#endif

