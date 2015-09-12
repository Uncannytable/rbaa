#!/bin/bash
for f in *.bc
do
	sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval $f -o /dev/null 2>>output.txt
done
