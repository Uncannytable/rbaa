//===--- Narrowing.cpp - Pass Implementation --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the implementation of the Range Based Pointer Ana-
/// lysis pass. It's used as a base pass for it's respective alias analysis
///
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "narrowing"
#include "RangeBasedPointerAnalysis.h"
#include "Primitives.h"
#include "Address.h"
#include "Narrowing.h"
#include "RangedPointer.h"
// llvm includes
#include "llvm/Pass.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/User.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
// libc includes
#include <set>
#include <map>
#include <fstream>
#include <string>
#include <system_error>
#include <deque>
#include <iostream>
#include <ctime>

using namespace llvm;

/// NarrowingOp class implementation
// Constructors
NarrowingOp* NarrowingOp::contextualize(Range* c)
{
  NarrowingOp* o = new NarrowingOp();
  o->cmp_op = cmp_op;
  o->cmp_v = cmp_v;
  Range r = *context + *c;
  o->context = new Range(r.getLower(), r.getUpper());
  return o;
}

//
NarrowingOp::~NarrowingOp()
{
	delete context;
}

