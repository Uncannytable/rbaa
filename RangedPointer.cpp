//===--- RangedPointer.cpp - Pass Implementation --------------===//
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
#define DEBUG_TYPE "ranged-pointer"
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

/// Globals
// SAGE Interface from the symbolic range analysis
extern SAGEInterface *SI;
// Primitives persistant data
extern Primitives* P;
// Symbolic Range Analysis
extern RangeAnalysis* RA;

/// RangedPointer class implementation
// Contructors
RangedPointer::RangedPointer(const Value* pointer)
{
	Pointer = pointer;
}

RangedPointer::RangedPointer(const Value* pointer, PointerTypes pointer_type)
{
  Pointer = pointer;
  PointerType = pointer_type;
}

RangedPointer::RangedPointer(RangedPointer* copy)
{
  Pointer = copy->Pointer;
  PointerType = copy->PointerType;
  /// TODO: Validate
  for(auto i : copy->Addresses)
  {
    Addresses.insert(new Address(i));
  }
  for(auto i : copy->Bases)
  {
    Bases.insert(i);
  }
}

// Processes a get element pointer into a new address of the ranged pointer
void RangedPointer::processGEP( RangedPointer* base, const Use* idx_begin, const Use* idx_end)
{
  const Value* base_ptr = base->getPointer();
  Expr lower_range(*SI, 0);
  Expr higher_range(*SI, 0);
  
  //Number of primitive elements
  Type* base_ptr_type = base_ptr->getType();
  int base_ptr_num_primitive = 
    P->getNumPrimitives(base_ptr_type->getPointerElementType());
    
  //parse first index
  Value* indx = idx_begin->get();
  
  if(ConstantInt* cint = dyn_cast<ConstantInt>(indx))
  {
    int constant = cint->getSExtValue();
    //updating lower and higher ranges
    lower_range = lower_range + Expr(*SI, base_ptr_num_primitive * constant);
    higher_range = higher_range + Expr(*SI, base_ptr_num_primitive * constant);
  }
  else //getStateOrInf
  {
    Range r = RA->getStateOrInf(indx);
    //updating lower and higher ranges
    lower_range = lower_range 
      + ( Expr(*SI, base_ptr_num_primitive) 
      * r.getLower() );
    higher_range = higher_range 
      + ( Expr(*SI, base_ptr_num_primitive) 
      * r.getUpper() );
  }
  
  //parse sequential indexes
  int index = 0;
  for(int i = 1; (idx_begin + i) != idx_end; i++)
  {
    //Calculating Primitive Layout
    base_ptr_type = P->getTypeInside(base_ptr_type, index);
    std::vector<int> base_ptr_primitive_layout = 
      P->getPrimitiveLayout(base_ptr_type);
      
    Value* indx = (idx_begin + i)->get();
    if(ConstantInt* cint = dyn_cast<ConstantInt>(indx))
    {
      int constant = cint->getSExtValue();
      
      lower_range = lower_range 
        + Expr(*SI, P->getSumBehind(base_ptr_primitive_layout, constant));
      
      higher_range = higher_range 
        + Expr(*SI, P->getSumBehind(base_ptr_primitive_layout, constant));
      
      index = constant;
    }
    else
    {
      Range r = RA->getStateOrInf(indx);
      
      if(r.getLower().isConstant())
      {
        lower_range = lower_range 
          + Expr( *SI, 
            P->getSumBehind(base_ptr_primitive_layout, 
              r.getLower().getInteger()) );
      }
      
      if(r.getUpper().isConstant())
      {
        higher_range = higher_range 
          + Expr( *SI, 
            P->getSumBehind(base_ptr_primitive_layout, 
              r.getUpper().getInteger()) );
      }
      else
      {
        higher_range = higher_range 
          + Expr( *SI, 
            P->getSumBehind(base_ptr_primitive_layout, 
              base_ptr_primitive_layout.size()) );
      }
      index = 0;
    }
  }
  new Address(this, base, new Range(lower_range, higher_range) ); 
}

//Creates the pointer's addresses and defines it's type
void RangedPointer::processInitialAddresses(RangeBasedPointerAnalysis* analysis)
{
  errs() << *Pointer << "\n";
  if(isa<const GlobalVariable>(*Pointer))
  {
    //errs() << "Global Variable.\n";
    PointerType = Alloc;  
  }
  else if(const Argument* p = dyn_cast<Argument>(Pointer))
  {
    if(p->getName().equals("argv"))
    {
      //errs() << "argv Parameter.\n";
      PointerType = Alloc;
    }
    else
    {
      errs() << "Argument Parameter.\n";
      PointerType = Phi;
      const Function* F = p->getParent();
      for(auto ui = F->user_begin(), ue = F->user_end(); ui != ue; ui++)
      {
        const User* u = *ui;
        if(const CallInst* caller = dyn_cast<CallInst>(u))
        {
          int anum = caller->getNumArgOperands();
          int ano = p->getArgNo();
          if(ano <= anum)
          {
            /// create address
            RangedPointer* Base = analysis->getRangedPointer
              (caller->getArgOperand(ano));
            errs() << "Base: "<< Base << "\n";
            new Address(this, Base, new Range(Expr(*SI, 0),Expr(*SI, 0)) );
            errs() << "x\n";
          }
          else
          {
            /// TODO: support standard values in cases where the argument
            /// has a standard value and does not appear in function call
            errs() << "!: ERROR (Not enough arguments):\n";
            errs() << *p << " " << ano << "\n";
            errs() << *u << "\n";
            PointerType = Unk;
          }
        }
      }
      
      
    }
  }
  else if(isa<const AllocaInst>(*Pointer))
  {
    //errs() << "Allocation Instruction.\n";
    PointerType = Alloc;
  }
  else if(const CallInst* p = dyn_cast<CallInst>(Pointer))
  {
    PointerType = Phi;
    Function* CF = p->getCalledFunction();
    if(CF)
    {
      if( strcmp( CF->getName().data(), "malloc") == 0
      or strcmp( CF->getName().data(), "calloc") == 0  )
      { //if is an allocation function
        //errs() << "Allocation Function Call.\n";
        PointerType = Alloc;
      }
      else if(strcmp( CF->getName().data(), "realloc") == 0)
      {
        /// realloc is of the same name as it's first argument
        //errs() << "Reallocation Function Call.\n";
        PointerType = Cont;
        const Value* BasePtr = p->getOperand(0);
        RangedPointer* Base = analysis->getRangedPointer(BasePtr);
        new Address(this, Base, new Range( Expr(*SI, 0),Expr(*SI, 0)) );
      }
      else
      { //else, addresses must be calculated
        //errs() << "Function Call.\n";
        for (auto i = inst_begin(CF), e = inst_end(CF); i != e; i++)
          if(isa<const ReturnInst>(*i))
          {
            /// create address
            const Value* RetPtr = ((ReturnInst*)&(*i))->getReturnValue();
            RangedPointer* Base = analysis->getRangedPointer(RetPtr);
            new Address(this, Base, new Range( Expr(*SI, 0),Expr(*SI, 0)) );
          }
        if(Addresses.empty())
          PointerType = Unk;
      }
    }
    else
    {
      errs() << "Unknown Function Call.\n";
      PointerType = Unk;
    }
  }
  else if(const BitCastInst* p = dyn_cast<BitCastInst>(Pointer))
  {
    //errs() << "Bitcast Instruction.\n";
    PointerType = Cont;
    /// create address
    Value* BasePtr = p->getOperand(0);
    RangedPointer* Base = analysis->getRangedPointer(BasePtr);
    new Address(this, Base, new Range( Expr(*SI, 0),Expr(*SI, 0)) );
  }
  else if(isa<const LoadInst>(*Pointer))
  {
    //errs() << "Load Instruction.\n";
    PointerType = Unk;
  }
  else if(const PHINode* p = dyn_cast<PHINode>(Pointer))
  {
    //errs() << "Phi Instruction.\n";
    PointerType = Phi;
    // create addresses
    unsigned int num = p->getNumIncomingValues();
    for (unsigned int i = 0; i < num; i++)
    {
      Value* BasePtr = p->getIncomingValue(i);
      RangedPointer* Base = analysis->getRangedPointer(BasePtr);
      new Address(this, Base, new Range( Expr(*SI, 0),Expr(*SI, 0)) );
    }  
  }
  else if(const GetElementPtrInst* p = dyn_cast<GetElementPtrInst>(Pointer))
  {
    //errs() << "GEP Instruction.\n";
    PointerType = Cont;
    const Value* BasePtr = p->getPointerOperand();
    RangedPointer* Base = analysis->getRangedPointer(BasePtr);
    processGEP(Base, p->idx_begin(), p->idx_end());
  }
  else if(const GEPOperator* p = dyn_cast<GEPOperator>(Pointer))
  {
    //errs() << "GEP Operator.\n";
    PointerType = Cont;
    const Value* BasePtr = p->getPointerOperand();
    RangedPointer* Base = analysis->getRangedPointer(BasePtr);
    processGEP(Base, p->idx_begin(), p->idx_end());
  }
  else if(isa<const ConstantPointerNull>(*Pointer))
  {
    //errs() << "Null Pointer.\n";
    PointerType = Null;
  }
  else if(const ConstantExpr* p = dyn_cast<ConstantExpr>(Pointer))
  {
    const char* operation = p->getOpcodeName();
    if(strcmp(operation, "bitcast") == 0)
    {
      //errs() << "Bitcast Constant Expression.\n";
      PointerType = Cont;
      /// create address
      Value* BasePtr = p->getOperand(0);
      RangedPointer* Base = analysis->getRangedPointer(BasePtr);
      new Address(this, Base, new Range( Expr(*SI, 0),Expr(*SI, 0)) );
    }
    else
    {
      PointerType = Unk;
      errs() << *p << " -> Unknown ConstantExpr\n";
    }
  }
  else if(isa<const Function>(*Pointer))
  {
    //errs() << "Function.\n";
    PointerType = Alloc;
  }
  else
  {
    PointerType = Unk;
    errs() << *Pointer << " -> Unknown Value\n";
  }
}

// Returns the pointer
const Value* RangedPointer::getPointer()
{
  return Pointer;
}

// Sets the pointer type
void RangedPointer::setPointerType(RangedPointer::PointerTypes pt)
{
  PointerType = pt;
}

// Returns the pointer's type
enum RangedPointer::PointerTypes RangedPointer::getPointerType()
{
 return PointerType;
}

// Returns the begining of the set of addresses
std::set<Address*>::iterator RangedPointer::addr_begin()
{
  return Addresses.begin();
}

// Returns the end of the set of addresses
std::set<Address*>::iterator RangedPointer::addr_end()
{
  return Addresses.end();
}

// Returns whether the pointer has addresses or not
bool RangedPointer::addr_empty()
{
  return Addresses.empty();
}

// Returns the begining of the set of bases
std::set<Address*>::iterator RangedPointer::bases_begin()
{
  return Bases.begin();
}

// Returns the end of the set of bases
std::set<Address*>::iterator RangedPointer::bases_end()
{
  return Bases.end();
}

// Prints the ranged pointer to errs()
void RangedPointer::print()
{
  errs() << "Pointer: " << *Pointer << "\n";
  
  errs() << "Type: ";
  if(PointerType == RangedPointer::Unk)
    errs() << "Unknown\n";
  else if(PointerType == RangedPointer::Alloc)
    errs() << "Allocation\n";
  else if(PointerType == RangedPointer::Phi)
    errs() << "Phi\n";
  else if(PointerType == RangedPointer::Cont)
    errs() << "Continuous\n";
  else if(PointerType == RangedPointer::Null)
    errs() << "Null\n";
  
  errs() << "{";
  for(auto i : Addresses)
  {
    errs() << " (";
    i->print();
    errs() << ") ";
  }
  errs() << "}\n";
}

// Returns whether the pointer is evil and wants to destroy humanity
bool RangedPointer::isEvil()
{
  if(getPointerType() == RangedPointer::Unk)
    return true;
  for(auto i : Addresses)
    if(i->getBase()->getPointerType() != RangedPointer::Alloc)
      return true;
  return false;
}

void RangedPointer::getUniquePath()
{
  RangedPointer* current = this;
  int index = 0;
  Range* range = new Range(Expr(*SI, 0),Expr(*SI, 0));
  while(true)
  {
    Path[current] = std::pair<int, Range*>(index, range);
    if(current->Addresses.size() == 1)
    {
      Address* addr = *(current->Addresses.begin());
      current = addr->getBase();
      index++;
      range = new Range(range->getLower()+addr->getOffset()->getLower(),
        range->getUpper()+addr->getOffset()->getUpper()); 
    }
    else
    {
      LocalTree = current;
      break;
    }
  }
}

