//===--- Primitives.h - Pass definition ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file contains the declaration of the primitives class for the
/// Ranged Based Alias Analysis
///
//===----------------------------------------------------------------------===//
#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include <vector>

namespace llvm {

/// Forward declarations
class Type;

/// Representation of types as sequences of primitive values
class Primitives
{
  public:
  //Holds a Primitive Layout for a determined Type
  struct PrimitiveLayout
  {
    Type * type;
    std::vector<int> layout;
    PrimitiveLayout(Type* ty, std::vector<int> lay)
    {
      type = ty;
      layout = lay;
    }
  };
  struct NumPrimitive
  {
    Type * type;
    int num;
    NumPrimitive(Type* ty, int n)
    {
      type = ty;
      num = n;
    }
  };
  std::vector<PrimitiveLayout*> PrimitiveLayouts;
  std::vector<NumPrimitive*> NumPrimitives;
  std::vector<int> getPrimitiveLayout(Type* type);
  int getNumPrimitives(Type* type);
  llvm::Type* getTypeInside(Type* type, int i);
  int getSumBehind(std::vector<int> v, unsigned int i);
};

}

#endif
