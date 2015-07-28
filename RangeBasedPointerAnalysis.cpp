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
SAGEInterface *SI;
Primitives* P;
RangeAnalysis* RA;

///Expr comparison functions
bool lt(const Expr &a, const Expr &b)
{
  /*if(!a.isEQ(Expr::getPlusInf(*SI)) and b.isEQ(Expr::getPlusInf(*SI)))
    return true;
  else if(a.isEQ(Expr::getMinusInf(*SI)) and !b.isEQ(Expr::getMinusInf(*SI)))
    return true;*/
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a < b).isEQ(Expr::getTrue(*SI));
}

bool le(const Expr &a, const Expr &b)
{
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a <= b).isEQ(Expr::getTrue(*SI));
}

bool gt(const Expr &a, const Expr &b)
{
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a > b).isEQ(Expr::getTrue(*SI));
}

bool ge(const Expr &a, const Expr &b)
{
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a >= b).isEQ(Expr::getTrue(*SI));
}

/// Primitives class implementation

//Returns the sum of previous elements of vector
int Primitives::getSumBehind(std::vector<int> v, unsigned int i)
{
  int s = 0;
  if(i > v.size())
    i = v.size();
  for(int j = i-1; j >= 0; j--)
    s += v[j];
  return s;
}

//Returns the type of the ith element inside type
Type* Primitives::getTypeInside(Type* type, int i)
{
  if(type->isPointerTy())
    return type->getPointerElementType();
  else if(type->isArrayTy())
    return type->getArrayElementType();
  else if(type->isStructTy())
    return type->getStructElementType(i);
  else if(type->isVectorTy())
    return type->getVectorElementType();
  else
    return NULL;  
}
  
//Returns the number of primitive elements of type
int Primitives::getNumPrimitives(Type* type)
{
  //Verifies if this number of primitives was calculated already
  for(unsigned int i = 0; i < NumPrimitives.size(); i++)
    if(NumPrimitives[i]->type == type)
      return NumPrimitives[i]->num;
  
  //if not
  int np = 1;
  if(type->isArrayTy())
  {
    int num = type->getArrayNumElements();
    Type* arrtype = type->getArrayElementType();
    int arrtypenum = getNumPrimitives(arrtype); 
    np = num * arrtypenum;
  }
  else if(type->isStructTy())
  {
    int num = type->getStructNumElements();
    np = 0;
    for(int i = 0; i < num; i++)
    {
      Type* structelemtype = type->getStructElementType(i);
      np += getNumPrimitives(structelemtype);
    }
  }
  else if(type->isVectorTy())
  {
    int num = type->getVectorNumElements();
    Type* arrtype = type->getVectorElementType();
    int arrtypenum = getNumPrimitives(arrtype); 
    np = num * arrtypenum;
  }
  
  NumPrimitives.insert(NumPrimitives.end(), new NumPrimitive(type, np));
  return np;
}

//Returns a vector with the primitive layout of type
std::vector<int> Primitives::getPrimitiveLayout(Type* type)
{
  //Verifies if this layout was calculated already
  for(unsigned int i = 0; i < PrimitiveLayouts.size(); i++)
    if(PrimitiveLayouts[i]->type == type)
      return PrimitiveLayouts[i]->layout;
  
  //if not
    
  if(type->isArrayTy())
  {
    int num = type->getArrayNumElements();
    std::vector<int> pm (num);
    Type* arrtype = type->getArrayElementType();
    int arrtypenum = getNumPrimitives(arrtype); 
    for(int i = 0; i < num; i++)
      pm[i] = arrtypenum;
    PrimitiveLayouts.insert(PrimitiveLayouts.end(), 
      new PrimitiveLayout(type, pm));
    return pm;
  }
  else if(type->isStructTy())
  {
    int num = type->getStructNumElements();
    std::vector<int> pm (num);
    for(int i = 0; i < num; i++)
    {
      Type* structelemtype = type->getStructElementType(i);
      pm[i] = getNumPrimitives(structelemtype);
    }
    PrimitiveLayouts.insert(PrimitiveLayouts.end(), 
      new PrimitiveLayout(type, pm));
    return pm;
  }
  else if(type->isVectorTy())
  {
    int num = type->getVectorNumElements();
    std::vector<int> pm (num);
    Type* arrtype = type->getVectorElementType();
    int arrtypenum = getNumPrimitives(arrtype); 
    for(int i = 0; i < num; i++)
      pm[i] = arrtypenum;
    PrimitiveLayouts.insert(PrimitiveLayouts.end(), 
      new PrimitiveLayout(type, pm));
    return pm;
  }
  else
  {
    std::vector<int> pm (1);
    pm[0] = 1;
    PrimitiveLayouts.insert(PrimitiveLayouts.end(), 
      new PrimitiveLayout(type, pm));
    return pm;
  }  
}

/// Address class implementation
Address::Address(RangedPointer* addressee, RangedPointer* base, Range* offset)
{
  Addressee = addressee;
  Base = base;
  Offset = offset;
  widened = false;
  Addressee->Addresses.insert(this);
  Base->Bases.insert(this);
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
}
Address::~Address()
{
  delete Offset;
  //for(auto i : narrowing_ops)
  //  delete i;
  Addressee->Addresses.erase(this);
  Base->Bases.erase(this);
}
RangedPointer* Address::getBase()
{
  return Base;
}
RangedPointer* Address::getAddressee()
{
  return Addressee;
}

void Address::setBase(RangedPointer* base)
{
  Base->Bases.erase(this);
  Base = base;
  Base->Bases.insert(this);
}

void Address::setOffset(Expr lower, Expr upper)
{
  Offset->setLower(lower);
  Offset->setUpper(upper);
}

Range* Address::getOffset()
{
  return Offset;
}
void Address::print()
{
  if(getBase()->getPointer()->getValueName() == NULL)
    errs() << *(getBase()->getPointer());
  else
    errs() << getBase()->getPointer()->getName();

  errs() << " + " << *(getOffset());
}

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


/// RangedPointer class implementation
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

void RangedPointer::processGEP( RangedPointer* base, 
  const Use* idx_begin, const Use* idx_end)
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
              r.getLower().getConstant()) );
      }
      
      if(r.getUpper().isConstant())
      {
        higher_range = higher_range 
          + Expr( *SI, 
            P->getSumBehind(base_ptr_primitive_layout, 
              r.getUpper().getConstant()) );
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
void RangedPointer::processInitialAddresses(RangeBasedPointerAnalysis* analysis)
{
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
      //errs() << "Argument Parameter.\n";
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
            new Address(this, Base, new Range(Expr(*SI, 0),Expr(*SI, 0)) );
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

const Value* RangedPointer::getPointer()
{
  return Pointer;
}

void RangedPointer::setPointerType(RangedPointer::PointerTypes pt)
{
  PointerType = pt;
}

enum RangedPointer::PointerTypes RangedPointer::getPointerType()
{
 return PointerType;
}

std::set<Address*>::iterator RangedPointer::addr_begin()
{
  return Addresses.begin();
}

std::set<Address*>::iterator RangedPointer::addr_end()
{
  return Addresses.end();
}

bool RangedPointer::addr_empty()
{
  return Addresses.empty();
}

std::set<Address*>::iterator RangedPointer::bases_begin()
{
  return Bases.begin();
}

std::set<Address*>::iterator RangedPointer::bases_end()
{
  return Bases.end();
}

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

bool RangedPointer::isEvil()
{
  if(getPointerType() == RangedPointer::Unk)
    return true;
  for(auto i : Addresses)
    if(i->getBase()->getPointerType() != RangedPointer::Alloc)
      return true;
  return false;
}

/// NarrowingOp class implementation
NarrowingOp* NarrowingOp::contextualize(Range* c)
{
  NarrowingOp* o = new NarrowingOp();
  o->cmp_op = cmp_op;
  o->cmp_v = cmp_v;
  Range r = *context + *c;
  o->context = new Range(r.getLower(), r.getUpper());
  return o;
}

NarrowingOp::~NarrowingOp()
{
  delete context;
}

/// RangeBasedPointerAnalysis class implementation
RangedPointer* RangeBasedPointerAnalysis::getRangedPointer(const Value* value)
{
  const Type *type = value->getType();
  // TODO: add assertion
  if(!type->isPointerTy())
    return NULL;
  if(RangedPointers[value] == NULL)
    RangedPointers[value] = new RangedPointer(value, RangedPointer::Unk);
  return RangedPointers[value];
}

void RangeBasedPointerAnalysis::printDOT(Module &M, std::string stage)
{
  std::string name = M.getModuleIdentifier();
  name += stage;
  name += ".dot";
  std::string er = "";
  raw_fd_ostream fs(name.data(), er, (sys::fs::OpenFlags)8);
  fs << "digraph grafico {\n";
  for(auto i : RangedPointers)
  {
    if(i.first->getValueName() == NULL)
      fs << "\"" << *(i.first) << "_" << i.first << "\" ";
    else
      fs << "\"" << (i.first->getName()) << "_" << i.first << "\" ";
    
    if(i.second->getPointerType() == RangedPointer::Unk)
      fs << "[shape=egg];\n";
    else if(i.second->getPointerType() == RangedPointer::Alloc)
      fs << "[shape=square];\n";
    else if(i.second->getPointerType() == RangedPointer::Phi)
      fs << "[shape=diamond];\n";
    else if(i.second->getPointerType() == RangedPointer::Cont)
      fs << "[shape=ellipse];\n";
    else if(i.second->getPointerType() == RangedPointer::Null)
      fs << "[shape=point];\n";
        
    for(std::set<Address*>::iterator j = i.second->addr_begin(),
      je = i.second->addr_end(); j != je; j++)
    {      
      if((*j)->getBase()->getPointer()->getValueName() == NULL)
        fs << "\"" << *((*j)->getBase()->getPointer()) << "_" << (*j)->getBase()->getPointer() << "\" -> ";
      else
        fs << "\"" << ((*j)->getBase()->getPointer()->getName()) << "_" << (*j)->getBase()->getPointer() << "\" -> ";
      
      if(i.first->getValueName() == NULL)
        fs << "\"" << *(i.first) << "_" << i.first << "\" [label=\"";
      else
        fs << "\"" << (i.first->getName()) << "_" << i.first << "\" [label=\"";
      
      if((*j)->widened) fs << "*";  
      fs << *((*j)->getOffset());
      
      for(auto z : (*j)->Narrowing_Ops)
      {
        fs<< "N{";
        if(z.second->cmp_op == CmpInst::ICMP_EQ)
          fs << "=";
        else if(z.second->cmp_op == CmpInst::ICMP_NE)
          fs << "!=";
        else if(z.second->cmp_op == CmpInst::ICMP_SLT)
          fs << "<";
        else if(z.second->cmp_op == CmpInst::ICMP_SLE)
          fs << "<=";
        else if(z.second->cmp_op == CmpInst::ICMP_SGT)
          fs << ">";
        else if(z.second->cmp_op == CmpInst::ICMP_SGE)
          fs << ">=";
        fs << ", " << z.second->cmp_v << "+" << *(z.second->context) << "]}";
      }
      
      fs << "\"";
      if(i.second->getPointerType() == RangedPointer::Cont)
        fs << ",color=red];\n";
      else fs << "];\n";
    }
  }
  fs << "}";
}

void RangeBasedPointerAnalysis::DFS_visit(RangedPointer* u, 
std::deque<RangedPointer*>* dqp)
{
  u->color = 1;
  for(std::set<Address*>::iterator i = u->bases_begin(), e = u->bases_end();
  i != e; i++)
  {
	  RangedPointer* nu = (*i)->getAddressee();
    if(nu->color == 0)
      DFS_visit(nu, dqp);
  }
  u->color = 2;
  dqp->push_front(u);
}

void RangeBasedPointerAnalysis::Phase1_DFS_visit(RangedPointer* u, 
std::deque<RangedPointer*>* dqp)
{
  u->color = 1;
  for(std::set<Address*>::iterator i = u->bases_begin(), e = u->bases_end();
  i != e; i++)
  {
	  RangedPointer* nu = (*i)->getAddressee();
    if(nu->color == 0 and nu->getPointerType() == RangedPointer::Cont)
      Phase1_DFS_visit(nu, dqp);
  }
  u->color = 2;
  dqp->push_front(u);
}

void RangeBasedPointerAnalysis::Phase2_DFS_visit1(RangedPointer* u, 
std::deque<RangedPointer*>* dqp)
{
  u->color = 1;
  for(std::set<Address*>::iterator i = u->bases_begin(), e = u->bases_end();
  i != e; i++)
  {
	  RangedPointer* nu = (*i)->getAddressee();
    if(nu->color == 0)
      Phase2_DFS_visit1(nu, dqp);
  }
  u->color = 2;
  dqp->push_front(u);
}

void RangeBasedPointerAnalysis::Phase2_DFS_visit2(RangedPointer* u, int scc, 
  int &n)
{
  u->color = 1;
  for(std::set<Address*>::iterator i = u->addr_begin(), e = u->addr_end();
  i != e; i++)
  {
	  RangedPointer* nu = (*i)->getBase();
    if(nu->color == 0)
      Phase2_DFS_visit2(nu, scc, n);
  }
  n++;
  u->color = 2;
  u->scc = scc;
}

void RangeBasedPointerAnalysis::SCC_DFS_visit(RangedPointer* u, 
std::deque<RangedPointer*>* dqp)
{
  u->color = 1;
  for(std::set<Address*>::iterator i = u->bases_begin(), e = u->bases_end();
  i != e; i++)
  {
	  RangedPointer* nu = (*i)->getAddressee();
    if(nu->color == 0 and u->scc == nu->scc)
      SCC_DFS_visit(nu, dqp);
  }
  u->color = 2;
  dqp->push_front(u);
}

void RangeBasedPointerAnalysis::PreNarrowing()
{
  //getting narrowing information
  for(auto i : RangedPointers)
  {
    if(const PHINode* phi = dyn_cast<PHINode>(i.first))
    {
      if (phi->getName().startswith("SSIfy_sigma")) 
      {
        //find branch
        BasicBlock* o_block = phi->getIncomingBlock(0);
        BasicBlock::iterator bi = o_block->end();
        bi--;
        BranchInst* br = (BranchInst*) &(*bi);
        Value* conditional = br->getCondition();
        ICmpInst* cmp_i;
        if(isa<ICmpInst>(*conditional)) cmp_i = (ICmpInst*) conditional;
        else continue;
        if(narrowing_data[br] == NULL)
        {
          narrowing_data[br] = new NarrowingData();
          narrowing_data[br]->cmp_v1 = cmp_i->getOperand(0);
          narrowing_data[br]->cmp_v2 = cmp_i->getOperand(1);
          narrowing_data[br]->cmp_op = cmp_i->getPredicate();
        }
        narrowing_data[br]->sigmas.insert(phi);       
      }
    }
  }
  
  //Associating narrowing ops
  
  for(auto i : narrowing_data)
  {
    BranchInst* br = (BranchInst*) i.first;
    ICmpInst* cmp_i = (ICmpInst*) br->getCondition();
    //for each sigma
    for(std::set<const PHINode*>::iterator ii = i.second->sigmas.begin(), 
    ee = i.second->sigmas.end(); ii != ee; ii++)
    {
      const PHINode* sigma = (const PHINode*) *ii;
      NarrowingOp* no = new NarrowingOp();
      no->context = new Range(Expr(*SI, 0));
      if(i.second->cmp_op == CmpInst::ICMP_EQ)
      {
        if(sigma->getParent() == br->getSuccessor(0))
        { //its the true sigma
          no->cmp_op = CmpInst::ICMP_EQ;
          if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_v = cmp_i->getOperand(0);
          }  
        }
        else if(sigma->getParent() == br->getSuccessor(1))
        { //its the false sigma
          no->cmp_op = CmpInst::ICMP_NE;
          if(sigma->getIncomingValue(0) == cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_v = cmp_i->getOperand(0);
          } 
        }
      }
      else if(i.second->cmp_op == CmpInst::ICMP_NE)
      {
      	if(sigma->getParent() == br->getSuccessor(0))
        { //its the true sigma
          no->cmp_op = CmpInst::ICMP_NE;
          if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_v = cmp_i->getOperand(0);
          }  
        }
        else if(sigma->getParent() == br->getSuccessor(1))
        { //its the false sigma
          no->cmp_op = CmpInst::ICMP_EQ;
          if(sigma->getIncomingValue(0) == cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_v = cmp_i->getOperand(0);
          } 
        }
      }
      else if(i.second->cmp_op == CmpInst::ICMP_UGT 
      or i.second->cmp_op == CmpInst::ICMP_SGT)
      {
      	if(sigma->getParent() == br->getSuccessor(0))
        { //its the true sigma
          if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_v = cmp_i->getOperand(1);
            no->cmp_op = CmpInst::ICMP_SGT;
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_v = cmp_i->getOperand(0);
            no->cmp_op = CmpInst::ICMP_SLT;
          }  
        }
        else if(sigma->getParent() == br->getSuccessor(1))
        { //its the false sigma
          if(sigma->getIncomingValue(0) == cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SLE;
			      no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SGE;
            no->cmp_v = cmp_i->getOperand(0);
          } 
        }
      }
      else if(i.second->cmp_op == CmpInst::ICMP_UGE 
      or i.second->cmp_op == CmpInst::ICMP_SGE)
      {
      	if(sigma->getParent() == br->getSuccessor(0))
        { //its the true sigma
          if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SGE;
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SLE;
            no->cmp_v = cmp_i->getOperand(0);
          }  
        }
        else if(sigma->getParent() == br->getSuccessor(1))
        { //its the false sigma
          if(sigma->getIncomingValue(0) == cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SLT;
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SGT;
            no->cmp_v = cmp_i->getOperand(0);
          } 
        }
      }
      else if(i.second->cmp_op == CmpInst::ICMP_ULT 
      or i.second->cmp_op == CmpInst::ICMP_SLT)
      {
      	if(sigma->getParent() == br->getSuccessor(0))
        { //its the true sigma
          if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SLT;
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SGT;
            no->cmp_v = cmp_i->getOperand(0);
          }  
        }
        else if(sigma->getParent() == br->getSuccessor(1))
        { //its the false sigma
          if(sigma->getIncomingValue(0) == cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SGE;
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SLE;
            no->cmp_v = cmp_i->getOperand(0);
          } 
        }
      }
      else if(i.second->cmp_op == CmpInst::ICMP_ULE 
      or i.second->cmp_op == CmpInst::ICMP_SLE)
      {
      	if(sigma->getParent() == br->getSuccessor(0))
        { //its the true sigma
          if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SLE;
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SGE;
            no->cmp_v = cmp_i->getOperand(0);
          }  
        }
        else if(sigma->getParent() == br->getSuccessor(1))
        { //its the false sigma
          if(sigma->getIncomingValue(0) == cmp_i->getOperand(0))
          { //its the left operand of cmp
            no->cmp_op = CmpInst::ICMP_SGT;
            no->cmp_v = cmp_i->getOperand(1);
          }
          else if(sigma->getIncomingValue(0) ==  cmp_i->getOperand(1))
          { //its the right operand of cmp
            no->cmp_op = CmpInst::ICMP_SLT;
            no->cmp_v = cmp_i->getOperand(0);
          } 
        }
      }
      
      //add narrowing op to sigma's ranged pointer
      for(std::set<Address*>::iterator ii = RangedPointers[sigma]->addr_begin(),
      ee = RangedPointers[sigma]->addr_end(); ii != ee; ii++)
      {
        (*ii)->Narrowing_Ops[sigma] = no;
      }
    } 
  }
}

/// \brief LLVM framework support function
void RangeBasedPointerAnalysis::getAnalysisUsage(AnalysisUsage &AU) const
{
  AU.setPreservesAll();
  AU.addRequired<RangeAnalysis>();
}
char RangeBasedPointerAnalysis::ID = 0;
static RegisterPass<RangeBasedPointerAnalysis> X("range-based-pa",
"Range Analysis based pointer analysis", false, false);

///////BEGINING/////////////////////////////////////////////////
/// \brief LLVM framework Main Function. Does the main analysis.
bool RangeBasedPointerAnalysis::runOnModule(Module &M)
{
  clock_t t;
  t = clock();
  int ninst = 0;
  int npointers = 0;

  RangeAnalysis &ra = getAnalysis<RangeAnalysis>();
  SI = &(ra.getSI());
  P = new Primitives();
  RA = &ra;
  
  std::cout << "Gathering pointers\n";
  
  /// The first step of the program consists on 
  /// gathering all pointers and stores
  std::set<const Value*> AllPointers;
  std::set<const StoreInst*> RelevantStores;
  /// Go through global variables to find arrays, structs and pointers
  for(auto i = M.global_begin(), e = M.global_end(); i != e; i++)
    //Since all globals are pointers, all are inserted
    AllPointers.insert(i);
  /// Go through all functions from the module
  for (auto F = M.begin(), Fe = M.end(); F != Fe; F++)
  {
    /// Go through parameters (add if they are pointers)
    for(auto i = F->arg_begin(), e = F->arg_end(); i != e; i++)
    {
      Type* const arg_type = i->getType();
      if(arg_type->isPointerTy())
        AllPointers.insert(i);
    }
     /// Run through instructions from function
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      ninst++;
      const Instruction* i = &(*I);
      const Type *type = i->getType();
      if(type->isPointerTy())
        AllPointers.insert(i);
      else if(const StoreInst* str_int = dyn_cast<StoreInst>(i))
      {
        const Type *type2 = str_int->getValueOperand()->getType();
        if(type2->isPointerTy())
          RelevantStores.insert(str_int);
      }
      //verify intruction operands
      for(auto oi = i->op_begin(), oe = i->op_end(); oi != oe; oi++)
      {
        const Value* oper = *oi;
        const Type *op_type = oper->getType();
        if(op_type->isPointerTy())
          AllPointers.insert(oper);
      }
    }
  }
  
  std::cout << "Building dependence graph\n";
  
  /// The second step of the program consists in building
  /// the pointer dependence graph
  npointers = AllPointers.size();
  for(auto *i : AllPointers)
  {
    RangedPointers[i] = new RangedPointer(i);
  }
  for(auto i : RangedPointers)
  {
    i.second->processInitialAddresses(this);
  }
  
  for(auto i : RangedPointers)
  {
    if(i.second->addr_empty())
      if(i.second->getPointerType() == RangedPointer::Cont
      or i.second->getPointerType() == RangedPointer::Phi)
        i.second->setPointerType(RangedPointer::Unk);
  }
  
  std::cout << "Connecting loads to stores\n";
  
  /// Connecting loads to stored pointers
  /*for(auto i : RangedPointers)
  {
    RangedPointer* rp = i.second;
    if(const LoadInst* li = dyn_cast<const LoadInst>(rp->getPointer()))
    {
      for(auto j : RelevantStores)
      {
        if(li->getParent()->getParent() == j->getParent()->getParent())
        {
          const Value* vop = j->getValueOperand();
          new Address(rp, RangedPointers[vop], 
            new Range( Expr(*SI, 0),Expr(*SI, 0)) );
          rp->setPointerType(RangedPointer::Phi);
        }
      } 
    }
  }*/
  
  std::cout << "Getting narrowing information\n";
  
  /// Getting narrowing information
  PreNarrowing();
  
  std::cout << "Printing number of unknown pointers\n";
  
  int unk = 0;
  for(auto i : RangedPointers) 
    if(i.second->getPointerType() == RangedPointer::Unk)
      unk++;
  errs() << "NUMBER OF UNKNOWN POINTERS: " << unk << "\n";
  std::cout << "NUMBER OF UNKNOWN POINTERS: " << unk << "\n";
  
  std::cout << "First topological ordering\n";  
  
  printDOT(M, std::string("_Phase0"));
  
  /// The third step of the program consists in running phase 1,
  /// i.e. normalizing Cont ranged pointers so they only have non
  /// Cont pointers as base
  
  /// DFS Topological ordering
  std::deque<RangedPointer*> dq;
  for(auto i : RangedPointers) i.second->color = 0;
  for(auto i : RangedPointers)
  {
    if(i.second->getPointerType() != RangedPointer::Cont 
      and i.second->color == 0)
    {
      Phase1_DFS_visit(i.second, &dq);
    }
  }
  
  std::cout << "Phase 1 normalization\n";
  
  /// Phase 1 core
  while(!dq.empty())
  {
    RangedPointer* rp = dq.front();
    dq.pop_front();
    
    if(rp->getPointerType() == RangedPointer::Cont)
    {
      std::set<Address*>::iterator iaddr = rp->addr_begin();
      RangedPointer* base = (*iaddr)->getBase();
      if(base->getPointerType() == RangedPointer::Cont)
      {
        std::set<Address*>::iterator giaddr = base->addr_begin();
        RangedPointer* granbase = (*giaddr)->getBase();
        
        /// TODO: Put assert here
        if(granbase->getPointerType() == RangedPointer::Cont)
          errs() << "PHASE 1 ERROR!\n";
          
        (*iaddr)->setBase(granbase);
        Range newOffset = *((*iaddr)->getOffset()) + *((*giaddr)->getOffset());
        (*iaddr)->setOffset( newOffset.getLower(), newOffset.getUpper() );
      }
    }
  }
  
  std::cout << "Saving phase 1\n";
  
  for(auto i : RangedPointers)
  {
    if(i.second->getPointerType() == RangedPointer::Cont)
    {
      Address* a = *(i.second->addr_begin());
      i.second->phase1_base = a->getBase();
      i.second->phase1_offset = new Range(
        a->getOffset()->getLower(), a->getOffset()->getUpper());
    }
    else
    {
      i.second->phase1_base = i.second;
      i.second->phase1_offset = new Range(Expr(*SI, 0));
    }
  }
  
  //printDOT(M, std::string("_Phase1"));
  
  std::cout << "Finding sccs\n";
  
  /// The fourth step of the program consists in running phase 2,
  /// i.e. normalizing all ranged pointers so they only have non
  /// Alloc and Unk pointers as bases
  
  /// finding sccs's
  dq.clear();
  for(auto i : RangedPointers)
  { 
    i.second->color = 0;
    i.second->scc = 0;
  }
  for(auto i : RangedPointers)
  {
    if(i.second->color == 0)
    {
      Phase2_DFS_visit1(i.second, &dq);
    }
  }
  
  int scc = 1;
  std::map<int,std::pair<RangedPointer*, int> > sccs;
  for(auto i : RangedPointers) i.second->color = 0;
  while(!dq.empty())
  {
    RangedPointer* rp = dq.front();
    dq.pop_front();
    if(rp->color == 0)
    {
      int n = 0;
      Phase2_DFS_visit2(rp, scc, n);
      sccs[scc] = std::pair<RangedPointer*, int>(rp, n);
      scc++;
    }
  }
  
  //std::cout << "SCCs: " << scc << "\n";
  
  /*for(int s = 0; s < scc; s++)
  {
    errs() << "SCC " << s << ":\n";
    for(auto i : RangedPointers)
    {
      if(i.second->scc == s)
      {
        if(i.first->getValueName() == NULL)
          errs() << *(i.first) << "_" << i.first << "\n";
        else
          errs() << (i.first->getName()) << "_" << i.first << "\n";
      }
    }
  }*/
  
  std::cout << "Resolving sccs\n";
  
  for(auto i : RangedPointers) i.second->color = 0;
  /// Resolving SCCs
  for(auto i : sccs)
  {
    //std::cout << i.first << "/" << scc << "\n"; 
    // if the scc has more then one pointer than there is a loop
    //if(i.second.second > 1)
    {
      dq.clear();
      SCC_DFS_visit(i.second.first, &dq);
      
      while(!dq.empty())
      {
        RangedPointer* rp = dq.front();
        dq.pop_front();
        
        //std::cout << rp << "\n";
        
        std::deque<Address*> ad;
        std::set<Address*> fn;
        
        std::pair<RangedPointer*, Range> p = 
          std::pair<RangedPointer*, Range>(rp,  Range(Expr(*SI, 0)));
        for(auto j = rp->addr_begin(), je = rp->addr_end(); j != je; j++)
        {
          ad.push_front(*j);
          (*j)->expanded.insert(p);
        }
        
        while(!ad.empty())
        {
          //std::cout << "ad: " << ad.size() << "\n";
          Address* addr = ad.front();
          ad.pop_front();
          
          if(addr->getBase()->scc == rp->scc)
          {
            //std::cout << "expand\n";
            addr->Expand(ad, fn);
          }
          else
          {
            //std::cout << "insert\n";
            fn.insert(addr);
          }
        }
      }
    }
  }
  
  //printDOT(M, std::string("_doneSCC"));
  
  std::cout << "Resolving whole graph\n";
  
  /// Resolving whole graph
  for(auto i : RangedPointers) i.second->color = 0;
  dq.clear();
  for(auto i : RangedPointers)
  {
    if(i.second->getPointerType() != RangedPointer::Cont 
      and i.second->color == 0)
    {
      DFS_visit(i.second, &dq);
    }
  }
  
  /*std::cout << "Expanded sizes:\n";
  for(auto i : RangedPointers)
  {
    for(auto j : i.second->Addresses)
    {
      std::cout << j->expanded.size();
      if(j->expanded.find(i.second) != j->expanded.end())
        std::cout << "SIM ";
      else
        std::cout << "NAO ";
    }
    std::cout << "\n";
  }
  int n;
  std::cin >> n;
  */
  
  while(!dq.empty())
  {
    RangedPointer* rp = dq.front();
    dq.pop_front();
    
    std::deque<Address*> ad;
    std::set<Address*> fn;
    
    for(auto j = rp->addr_begin(), je = rp->addr_end(); j != je; j++)
      ad.push_front(*j);
    
    while(!ad.empty())
    {
      Address* addr = ad.front();
      ad.pop_front();
      
      if(addr->getBase() == addr->getAddressee())
      {
        delete addr;
      }
      else if(addr->getBase()->getPointerType() == RangedPointer::Phi 
      or addr->getBase()->getPointerType() == RangedPointer::Cont)
      {
        //std::cout << "expand ";
        addr->Expand(ad, fn);
      }
      else
      {
        //std::cout << "insert\n";
        fn.insert(addr);
      }
    }
  }
  
  std::cout << "Applying narrowing\n";
  
  //printDOT(M, std::string("_Phase2preNarrowing"));
  
  /// Applying Narrowing
  for(auto i : RangedPointers)
  {
    for(auto j = i.second->addr_begin(), je = i.second->addr_end(); 
    j != je; j++)
    {
      Address* a = *j;
      if(!(*j)->Narrowing_Ops.empty())
      {
        Range narrowed = Range(a->getOffset()->getLower(), 
          a->getOffset()->getUpper());
        
        std::pair<bool,bool> growth;
        if(narrowed.getUpper().isEQ(Expr::getPlusInf(*SI)))
          growth.second = true;
        if(narrowed.getLower().isEQ(Expr::getMinusInf(*SI)))
          growth.first = true;
        
        for(auto z : a->Narrowing_Ops)
        {
          if(z.second->cmp_op == CmpInst::ICMP_EQ)
          {
            //errs() << "=\n";
            RangedPointer* rp = RangedPointers[z.second->cmp_v];
            for(auto w = rp->addr_begin(), we = rp->addr_end(); w != we; w++)
            {
              Expr dw = (*w)->getOffset()->getLower() + z.second->context->getUpper();
              if(!narrowed.getUpper().isConstant() and !dw.isConstant())
                narrowed.setUpper(narrowed.getUpper().min(dw));
              else if(gt(narrowed.getUpper(), dw))
                narrowed.setUpper(dw);
                
              //narrowed.setUpper(narrowed.getUpper().min(dw));
              
              Expr up = (*w)->getOffset()->getUpper() + z.second->context->getLower();
              if(!narrowed.getLower().isConstant() and !up.isConstant())
                narrowed.setLower(narrowed.getLower().max(up));
              else if(lt(narrowed.getLower(), up))
                narrowed.setLower(up);
              
              //narrowed.setLower(narrowed.getLower().max(up));
            }
          }
          else if(z.second->cmp_op == CmpInst::ICMP_NE)
          {
            //errs() << "!=\n";
            RangedPointer* rp = RangedPointers[z.second->cmp_v];
            for(auto w = rp->addr_begin(), we = rp->addr_end(); w != we; w++)
            {
              if(a->widened)
              {
                if(growth.second)
                {
                  Expr dw = (*w)->getOffset()->getLower() + z.second->context->getUpper();
                  if(!narrowed.getUpper().isConstant() and !dw.isConstant())
                    narrowed.setUpper(narrowed.getUpper().min(dw - 1));
                  else if(ge(narrowed.getUpper(), dw))
                    narrowed.setUpper(dw - 1);
                    
                  //narrowed.setUpper(narrowed.getUpper().min(dw - 1));
                }
                else if(growth.first)
                {
                  Expr up = (*w)->getOffset()->getUpper() + z.second->context->getLower();
                  if(!narrowed.getLower().isConstant() and !up.isConstant())
                    narrowed.setLower(narrowed.getLower().max(up + 1));
                  else if(le(narrowed.getLower(), up))
                    narrowed.setLower(up + 1);
                    
                  //narrowed.setLower(narrowed.getLower().max(up + 1));
                }
              }
            }
          }
          else if(z.second->cmp_op == CmpInst::ICMP_SLT)
          {
            //errs() << "<\n";
            RangedPointer* rp = RangedPointers[z.second->cmp_v];
            for(auto w = rp->addr_begin(), we = rp->addr_end(); w != we; w++)
            {
              Expr dw = (*w)->getOffset()->getLower() + z.second->context->getUpper();
              if(!narrowed.getUpper().isConstant() and !dw.isConstant())
                narrowed.setUpper(narrowed.getUpper().min(dw - 1));
              else if(ge(narrowed.getUpper(), dw))
                narrowed.setUpper(dw - 1);
              
              //narrowed.setUpper(narrowed.getUpper().min(dw - 1));
            }
          }
          else if(z.second->cmp_op == CmpInst::ICMP_SLE)
          {
            //errs() << "<=\n";
            RangedPointer* rp = RangedPointers[z.second->cmp_v];
            for(auto w = rp->addr_begin(), we = rp->addr_end(); w != we; w++)
            {
              Expr dw = (*w)->getOffset()->getLower() + z.second->context->getUpper();
              if(!narrowed.getUpper().isConstant() and !dw.isConstant())
                narrowed.setUpper(narrowed.getUpper().min(dw));
              else if(gt(narrowed.getUpper(), dw))
                narrowed.setUpper(dw);
            
              //narrowed.setUpper(narrowed.getUpper().min(dw));
            }
          }
          else if(z.second->cmp_op == CmpInst::ICMP_SGT)
          {
            //errs() << ">\n";
            RangedPointer* rp = RangedPointers[z.second->cmp_v];
            for(auto w = rp->addr_begin(), we = rp->addr_end(); w != we; w++)
            {
              Expr up = (*w)->getOffset()->getUpper() + z.second->context->getLower();
              if(!narrowed.getLower().isConstant() and !up.isConstant())
                narrowed.setLower(narrowed.getLower().max(up + 1));
              else if(le(narrowed.getLower(), up))
                narrowed.setLower(up + 1);
              
              //narrowed.setLower(narrowed.getLower().max(up + 1));
            }
          }
          else if(z.second->cmp_op == CmpInst::ICMP_SGE)
          {
            //errs() << ">=\n";
            RangedPointer* rp = RangedPointers[z.second->cmp_v];
            for(auto w = rp->addr_begin(), we = rp->addr_end(); w != we; w++)
            {
              Expr up = (*w)->getOffset()->getUpper() + z.second->context->getLower();
              if(!narrowed.getLower().isConstant() and !up.isConstant())
                narrowed.setLower(narrowed.getLower().max(up));
              else if(lt(narrowed.getLower(), up))
                narrowed.setLower(up);
              
              //narrowed.setLower(narrowed.getLower().max(up));
            }
          }
          
        } 
        a->Narrowing_Ops.clear();
        a->setOffset(narrowed.getLower(), narrowed.getUpper());
      }
    }
  }
  
  std::cout << "Adding self to base pointers\n";
  
  printDOT(M, std::string("_Phase2"));
  
  //Adding self to base pointers
  for(auto i : RangedPointers)
    if(i.second->addr_empty())
      new Address(i.second, i.second, new Range(Expr(*SI, 0),Expr(*SI, 0)));

  //for(auto i : RangedPointers)
  //i.second->print();
  
  //errs() << "Control flow reached the end.\n";
  
  t = clock() - t;
  errs() << ninst << " Instrucoes\n";
  errs() << npointers << " Ponteiros\n";
  errs() << (((float)t)/CLOCKS_PER_SEC) << " Tempo\n";
  
  return false;
}

