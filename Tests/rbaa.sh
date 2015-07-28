#!/bin/bash
~/Ecosoc/llvm-3.5.0/lib/Ecosoc/llvm-sra/SAGE/bin/sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval $1.ssify.bc -o $1.redef.bc 
~/Ecosoc/llvm-3.5.0/Release+Asserts/bin/llvm-dis $1.redef.bc -o $1.redef.ll
