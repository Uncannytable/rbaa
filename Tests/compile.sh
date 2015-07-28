#!/bin/bash
~/Ecosoc/llvm-3.5.0/Release+Asserts/bin/clang -c -emit-llvm $1.c -o $1.bc
~/Ecosoc/llvm-3.5.0/Release+Asserts/bin/llvm-dis $1.bc -o $1.clang.ll
~/Ecosoc/llvm-3.5.0/lib/Ecosoc/llvm-sra/SAGE/bin/sage-opt -load SSIfy.so -mem2reg -instnamer -break-crit-edges -ssify -set 1000  $1.bc -o $1.ssify.bc
~/Ecosoc/llvm-3.5.0/Release+Asserts/bin/llvm-dis $1.ssify.bc -o $1.ssify.ll
