#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval $1.ssify.bc -o $1.redef.bc 
llvm-dis $1.redef.bc -o $1.redef.ll
