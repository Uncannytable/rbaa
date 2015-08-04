#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load BadAliasAnalysis.so -no-aa -tbaa -targetlibinfo -badaa -notti -verify -simplifycfg -domtree -sroa -early-cse -lower-expect $1.redef.bc -o $1.O3.badaa.bc

sage-opt -load Python.so -load SAGE.so -load SRA.so -load BadAliasAnalysis.so -targetlibinfo -no-aa -tbaa -badaa -notti -verify -ipsccp -globalopt -deadargelim -instcombine -simplifycfg -basiccg -prune-eh -inline-cost -inline -functionattrs -argpromotion -sroa -domtree -early-cse -lazy-value-info -jump-threading -correlated-propagation -simplifycfg -instcombine -tailcallelim -simplifycfg -reassociate -domtree -loops -loop-simplify -lcssa -loop-rotate -licm -loop-unswitch -instcombine -scalar-evolution -lcssa -indvars -loop-idiom -loop-deletion -loop-unroll -memdep -mldst-motion -domtree -memdep -gvn -memdep -memcpyopt -sccp -instcombine -lazy-value-info -jump-threading -correlated-propagation -domtree -memdep -dse -loops -scalar-evolution -slp-vectorizer -adce -simplifycfg -instcombine -barrier -domtree -loops -loop-simplify -lcssa -branch-prob -block-freq -scalar-evolution -loop-vectorize -instcombine -simplifycfg -domtree -loops -loop-simplify -lcssa -scalar-evolution -loop-unroll -strip-dead-prototypes -globaldce -constmerge -verify -verify $1.O3.badaa.bc -o $1.O3.badaa.bc

llvm-dis $1.O3.badaa.bc -o $1.O3.badaa.ll
