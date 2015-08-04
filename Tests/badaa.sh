#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load BadAliasAnalysis.so -badaa -aa-eval $1.redef.bc -o /dev/null
