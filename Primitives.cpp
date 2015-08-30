//===--- Primitives.cpp - Pass Implementation --------------===//
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
#define DEBUG_TYPE "primitives"
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
