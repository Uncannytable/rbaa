//===--- Address.cpp - Pass Implementation --------------===//
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
#define DEBUG_TYPE "address"
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

/// Address class implementation
// Constructors
Address::Address(RangedPointer* addressee, RangedPointer* base, Range* offset)
{
  Addressee = addressee;
  Base = base;
  Offset = offset;
  widened = false;
  Addressee->Addresses.insert(this);
  Base->Bases.insert(this);
  
  if(isa<const Argument>(*(Base->getPointer())))
  	argument = true;
  else
  	argument = false;
}

Address::Address(Address* copy)
{
  Base = copy->Base;
  Addressee = copy->Addressee;
  Offset = new Range(copy->Offset->getLower(), copy->Offset->getUpper());
  expanded = copy->expanded;
  widened = copy->widened;
  Addressee->Addresses.insert(this);
  Base->Bases.insert(this);
  
  argument = copy->argument;
}

Address::~Address()
{
  delete Offset;
  //for(auto i : narrowing_ops)
  //  delete i;
  Addressee->Addresses.erase(this);
  Base->Bases.erase(this);
}

// Returns the base pointer for this address
RangedPointer* Address::getBase()
{
	return Base;
}

//Returns the addressee pointer for this address
RangedPointer* Address::getAddressee()
{
  return Addressee;
}

// Set the base pointer for this address
void Address::setBase(RangedPointer* base)
{
  Base->Bases.erase(this);
  Base = base;
  Base->Bases.insert(this);
}

// Set the offset for this address
void Address::setOffset(Expr lower, Expr upper)
{
  Offset->setLower(lower);
  Offset->setUpper(upper);
}

// Returns the offset for this address
Range* Address::getOffset()
{
  return Offset;
}

// Prints the address to errs()
void Address::print()
{
  if(getBase()->getPointer()->getValueName() == NULL)
    errs() << *(getBase()->getPointer());
  else
    errs() << getBase()->getPointer()->getName();

  errs() << " + " << *(getOffset());
}

// Expands an address to addresses using it's base pointer's base pointers as 
// the new addresses' base pointers
void Address::Expand(std::deque<Address*> &ad, std::set<Address*> &fn)
{
	if(expanded.find(Base) == expanded.end())
  { //if Base hasn't been expanded already
    //std::cout << "new\n";
    std::set<Address*> aux = Base->Addresses;
    for(auto i = aux.begin(), ie = aux.end(); i != ie; i++)
    {
      Range r = *((*i)->Offset) + *Offset;
      //if(Addressee == Base) std::cout << "CASE\n";
      Address* NewAddress = new Address(Addressee, (*i)->Base, 
      new Range(r.getLower(), r.getUpper()) );
      
      if(argument) NewAddress->argument = true;
      else if((*i)->argument) NewAddress->argument = true;
      
      NewAddress->expanded = expanded;
      NewAddress->expanded.insert(
      std::pair<RangedPointer*, Range>(Base, 
        Range(Offset->getLower(), Offset->getUpper()) ));
        
      if((*i)->widened)
      {  
        NewAddress->widened = (*i)->widened;
      }
      if(widened)
      {  
        NewAddress->widened = widened;
      }
        
      NewAddress->Narrowing_Ops = Narrowing_Ops;
      for(auto j : (*i)->Narrowing_Ops)
      {
        //NarrowingOp* nop = j.second->contextualize(Offset);
        if(NewAddress->Narrowing_Ops[j.first] == NULL)
          NewAddress->Narrowing_Ops[j.first] = j.second->contextualize(Offset);
      }
      ad.push_front(NewAddress);
    }
    
  }
  else
  { //if it has, there must be widening
    //std::cout << "old\n";
    Range r = Range(Expr(*SI,0));
    //std::cout << "first\n";
    for(auto i : ad)
    {
      i->widened = true;
      //std::cout << " no: " << narrowing_ops.size() << "\n";
      for(auto j : Narrowing_Ops)
        i->Narrowing_Ops[j.first] = j.second->contextualize(&r);
      //std::cout << "  e: " << expanded.size() << "\n";
      for(auto j : expanded)
        i->expanded.insert(j);
      
      if((expanded.at(Base).getLower() > Offset->getLower()).isEQ(Expr::getTrue(*SI)))
      {
        i->setOffset(Expr::getMinusInf(*SI), expanded.at(Base).getUpper());
      }
      else if((expanded.at(Base).getUpper() < Offset->getUpper()).isEQ(Expr::getTrue(*SI)))
      {
        i->setOffset(expanded.at(Base).getLower(), Expr::getPlusInf(*SI));
      }
      else
      {
        i->setOffset(Expr::getMinusInf(*SI), Expr::getPlusInf(*SI));
      }
    }
    //std::cout << "second\n";
    for(auto i : fn)
    {
      i->widened = true;
      for(auto j : Narrowing_Ops)
        i->Narrowing_Ops[j.first] = j.second->contextualize(&r);
      for(auto j : expanded)
        i->expanded.insert(j);
      
      if((expanded.at(Base).getLower() > Offset->getLower()).isEQ(Expr::getTrue(*SI)))
        i->setOffset(Expr::getMinusInf(*SI), expanded.at(Base).getUpper());
      else if((expanded.at(Base).getUpper() < Offset->getUpper()).isEQ(Expr::getTrue(*SI)))
        i->setOffset(expanded.at(Base).getLower(), Expr::getPlusInf(*SI));
      else
        i->setOffset(Expr::getMinusInf(*SI), Expr::getPlusInf(*SI));
    }
  }
  delete this;
}
