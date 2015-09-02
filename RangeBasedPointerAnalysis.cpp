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
#include "llvm/Support/Debug.h"
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
SAGEInterface *SI;
// Primitives persistant data
Primitives* P;
// Symbolic Range Analysis
RangeAnalysis* RA;

///Expr comparison functions
// less than
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

// less or equal
bool le(const Expr &a, const Expr &b)
{
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a <= b).isEQ(Expr::getTrue(*SI));
}

// greater than
bool gt(const Expr &a, const Expr &b)
{
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a > b).isEQ(Expr::getTrue(*SI));
}

// greater or equal
bool ge(const Expr &a, const Expr &b)
{
  if(!a.isConstant() and b.isConstant())
    return true;
  else if(a.isConstant() and !b.isConstant())
    return true;
  else
    return (a >= b).isEQ(Expr::getTrue(*SI));
}

/// RangeBasedPointerAnalysis class implementation
// Returns the ranged pointer corresponding to such Value
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

// Prints a DOT graph of the dependence graph
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
        fs << "\"" << *((*j)->getBase()->getPointer()) << "_" << 
        (*j)->getBase()->getPointer() << "\" -> ";
      else
        fs << "\"" << ((*j)->getBase()->getPointer()->getName()) << "_" << 
        (*j)->getBase()->getPointer() << "\" -> ";
      
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

// DFS based on color
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

// DFS based on color and Continous type
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

// DFS based on color
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

// DFS visit for calculating scc
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

//DFS based on color and scc
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

// Function that collects narrowing information
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
  AU.addRequired<SymbolicRangeAnalysis>();
}
char RangeBasedPointerAnalysis::ID = 0;
static RegisterPass<RangeBasedPointerAnalysis> X("range-based-pa", 
	"Range Analysis based pointer analysis", false, false);

/// \brief LLVM framework Main Function. Does the main analysis.
bool RangeBasedPointerAnalysis::runOnModule(Module &M)
{
	//Settin up
	clock_t t;
  t = clock();
  int ninst = 0;
  int npointers = 0;

	RangeAnalysis* ra;
	for (auto F = M.begin(), Fe = M.end(); F != Fe; F++)
  {
  	if(F->begin() != F->end())
  		ra = &(getAnalysis<RangeAnalysis>(*F));
  }
  SI = &(ra->getSI());
  P = new Primitives();
  RA = ra;
  
  DEBUG_WITH_TYPE("phases", errs() << "Gathering module's pointers\n");
  /// The first step of the program consists on 
  /// gathering all pointers and stores
  gatherPointers(M, ninst);
  
  DEBUG_WITH_TYPE("phases", errs() << "Building dependence graph\n");
  /// The second step of the program consists in building
  /// the pointer dependence graph
  buildDependenceGraph(npointers);
  
  //DEBUG_WITH_TYPE("phases", errs() << "Connecting loads to stores\n");
  /// Connecting loads to stored pointers
  //loadStoreConnect();
  
  DEBUG_WITH_TYPE("phases", errs() << "Getting narrowing information\n");
  /// Getting narrowing information
  PreNarrowing();
  
  DEBUG_WITH_TYPE("phases", errs() << "Printing number of unknown pointers\n");
  int unk = 0;
  for(auto i : RangedPointers) 
    if(i.second->getPointerType() == RangedPointer::Unk)
      unk++;
  errs() << "NUMBER OF UNKNOWN POINTERS: " << unk << "\n";
  
  DEBUG_WITH_TYPE("dot_graphs", printDOT(M, std::string("_Phase0")));
  
  /// Local analysis
  for(auto i : RangedPointers)
  {
    i.second->getUniquePath();
  }
  
  DEBUG_WITH_TYPE("dot_graphs", printDOT(M, std::string("_Phase1")));
  /// Global Analysis
    
  /// Normalizing all ranged pointers so they only have non
  /// Alloc and Unk pointers as bases
  DEBUG_WITH_TYPE("phases", errs() << "Finding sccs\n");
  std::map<int,std::pair<RangedPointer*, int> > sccs = findSCCs();
  
  DEBUG_WITH_TYPE("phases", errs() << "Resolving sccs\n");
  resolveSCCs(sccs);
  
  DEBUG_WITH_TYPE("dot_graphs", printDOT(M, std::string("_doneSCC")));
  DEBUG_WITH_TYPE("phases", errs() << "Resolving whole graph\n");
  resolveWholeGraph();
  
  DEBUG_WITH_TYPE("dot_graphs", printDOT(M, std::string("_beforeNarrowing")));
  DEBUG_WITH_TYPE("phases", errs() << "Applying narrowing\n");
  applyNarrowing();
  
  
  DEBUG_WITH_TYPE("dot_graphs", printDOT(M, std::string("_Phase2")));

	DEBUG_WITH_TYPE("phases", errs() << "Adding self to base pointers\n");
  //Adding self to base pointers
  for(auto i : RangedPointers)
    if(i.second->addr_empty())
      new Address(i.second, i.second, new Range(Expr(*SI, 0),Expr(*SI, 0)));

	DEBUG_WITH_TYPE("phases", errs() << "Control flow reached the end.\n");
  t = clock() - t;
  errs() << ninst << " Instrucoes\n";
  errs() << npointers << " Ponteiros\n";
  errs() << (((float)t)/CLOCKS_PER_SEC) << " Tempo\n";
return false;
}

//Function that gather the module's pointers
void RangeBasedPointerAnalysis::gatherPointers(Module &M, int &ninst)
{
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
}

// Builds the pointer dependence graph
void RangeBasedPointerAnalysis::buildDependenceGraph(int &npointers)
{
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
}

/// Connects loads to stored pointers
void RangeBasedPointerAnalysis::loadStoreConnect()
{
	for(auto i : RangedPointers)
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
  }
}

// Finds sccs in the dependence graph
std::map<int,std::pair<RangedPointer*, int> > 
  RangeBasedPointerAnalysis::findSCCs()
{
	std::deque<RangedPointer*> dq;
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
  
  return sccs;
}

// Resolves the sccs in the dependence graph
void RangeBasedPointerAnalysis::resolveSCCs
	(std::map<int,std::pair<RangedPointer*, int> > sccs)
{
 std::deque<RangedPointer*> dq;
 for(auto i : RangedPointers) i.second->color = 0;
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
}

// Resolves the whole graph
void RangeBasedPointerAnalysis::resolveWholeGraph()
{
	std::deque<RangedPointer*> dq;
  for(auto i : RangedPointers) i.second->color = 0;
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
}

// Applies the narrowing operations
void RangeBasedPointerAnalysis::applyNarrowing()
{
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
}
