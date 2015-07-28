#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval $1.redef.bc -o /dev/null
