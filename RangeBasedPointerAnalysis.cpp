//===--- RangeBasedPointerAnalysis.cpp - Pass Implementation --------------===//
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
#define DEBUG_TYPE "range-based-pa"
#include "RangeBasedPointerAnalysis.h"
//
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
//
#include <set>
#include <map>
#include <fstream>
#include <string>
#include <system_error>
#include <deque>
#include <iostream>
#include <ctime>

using namespace llvm;

/// Globals
//
SAGEInterface *SI;
//
Primitives* P;
//
RangeAnalysis* RA;

///Expr comparison functions
//
bool lt(const Expr &a, const Expr &b)
{return false;}
//
bool le(const Expr &a, const Expr &b)
{return false;}
//
bool gt(const Expr &a, const Expr &b)
{return false;}
//
bool ge(const Expr &a, const Expr &b)
{return false;}

/// Primitives class implementation
//Returns the sum of previous elements of vector
int Primitives::getSumBehind(std::vector<int> v, unsigned int i)
{return 0;}
//Returns the type of the ith element inside type
Type* Primitives::getTypeInside(Type* type, int i)
{return NULL;}
//Returns the number of primitive elements of type
int Primitives::getNumPrimitives(Type* type)
{return 0;}
//Returns a vector with the primitive layout of type
std::vector<int> Primitives::getPrimitiveLayout(Type* type)
{return std::vector<int>();}

/// Address class implementation
//
Address::Address(RangedPointer* addressee, RangedPointer* base, Range* offset)
{}
Address::Address(Address* copy)
{}
Address::~Address()
{}
//
RangedPointer* Address::getBase()
{return NULL;}
//
RangedPointer* Address::getAddressee()
{return NULL;}
//
void Address::setBase(RangedPointer* base)
{}
//
void Address::setOffset(Expr lower, Expr upper)
{}
//
Range* Address::getOffset()
{return NULL;}
//
void Address::print()
{}
//
void Address::Expand(std::deque<Address*> &ad, std::set<Address*> &fn)
{}

/// RangedPointer class implementation
RangedPointer::RangedPointer(const Value* pointer)
{}
RangedPointer::RangedPointer(const Value* pointer, PointerTypes pointer_type)
{}
RangedPointer::RangedPointer(RangedPointer* copy)
{}
//
void RangedPointer::processGEP( RangedPointer* base, const Use* idx_begin, const Use* idx_end)
{}
void RangedPointer::processInitialAddresses(RangeBasedPointerAnalysis* analysis)
{}
//
const Value* RangedPointer::getPointer()
{return NULL;}
//
void RangedPointer::setPointerType(RangedPointer::PointerTypes pt)
{}
//
enum RangedPointer::PointerTypes RangedPointer::getPointerType()
{return NULL;}
//
std::set<Address*>::iterator RangedPointer::addr_begin()
{return std::set<Address*>::iterator();}
//
std::set<Address*>::iterator RangedPointer::addr_end()
{return std::set<Address*>::iterator();}
//
bool RangedPointer::addr_empty()
{return false;}
//
std::set<Address*>::iterator RangedPointer::bases_begin()
{return std::set<Address*>::iterator();}
//
std::set<Address*>::iterator RangedPointer::bases_end()
{return std::set<Address*>::iterator();}
//
void RangedPointer::print()
{}
//
bool RangedPointer::isEvil()
{return false;}

/// NarrowingOp class implementation
//
NarrowingOp* NarrowingOp::contextualize(Range* c)
{return NULL;}
//
NarrowingOp::~NarrowingOp()
{}

/// RangeBasedPointerAnalysis class implementation
//
RangedPointer* RangeBasedPointerAnalysis::getRangedPointer(const Value* value)
{return NULL;}
//
void RangeBasedPointerAnalysis::printDOT(Module &M, std::string stage)
{}
//
void RangeBasedPointerAnalysis::DFS_visit(RangedPointer* u, std::deque<RangedPointer*>* dqp)
{}
//
void RangeBasedPointerAnalysis::Phase1_DFS_visit(RangedPointer* u, std::deque<RangedPointer*>* dqp)
{}
//
void RangeBasedPointerAnalysis::Phase2_DFS_visit1(RangedPointer* u, std::deque<RangedPointer*>* dqp)
{}
//
void RangeBasedPointerAnalysis::Phase2_DFS_visit2(RangedPointer* u, int scc, int &n)
{}
//
void RangeBasedPointerAnalysis::SCC_DFS_visit(RangedPointer* u, std::deque<RangedPointer*>* dqp)
{}
//
void RangeBasedPointerAnalysis::PreNarrowing()
{}

/// \brief LLVM framework support function
void RangeBasedPointerAnalysis::getAnalysisUsage(AnalysisUsage &AU) const
{}
char RangeBasedPointerAnalysis::ID = 0;
static RegisterPass<RangeBasedPointerAnalysis> X("range-based-pa", "Range Analysis based pointer analysis", false, false);

/// \brief LLVM framework Main Function. Does the main analysis.
bool RangeBasedPointerAnalysis::runOnModule(Module &M)
{return false;}

