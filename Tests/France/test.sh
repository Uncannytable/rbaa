#!/bin/bash

/home/vitor/Ecosoc/llvm-3.5.0/Release+Asserts/bin/clang -c -emit-llvm alias.c -o alias.bc
/home/vitor/Ecosoc/llvm-3.5.0/lib/Ecosoc/llvm-sra/SAGE/bin/sage-opt 
