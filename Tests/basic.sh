#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info $1.redef.bc -o /dev/null
