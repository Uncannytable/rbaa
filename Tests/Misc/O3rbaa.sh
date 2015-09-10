# O3 optimizations with rbaa, which place, early-cse deleated

#!/bin/bash


name=${1%.*}
rm $name.exe
rm $name.rbc
rm $name.opt.rbc


lib_path="/home/mmaalej/Tools/llvm-3.5.0/src/Release+Asserts/lib"
ssify_so="SSIfy.so"
python_so="Python.so"
sage_so="SAGE.so"
rbaa_so="RangeBasedAliasAnalysis.so"
sra_so="SRA.so"
licm2_so="LICM2.so"



clang -c -emit-llvm $1 -o $name.bc
opt -mem2reg -instnamer $name.bc -o $name.rbc
sage-opt -load $lib_path/$ssify_so -break-crit-edges -ssify -set 1000  $name.rbc -o $name.rbc
sage-opt -stats -load $lib_path/$python_so -load $lib_path/$sage_so -load $lib_path/$sra_so -load $lib_path/$rbaa_so -load $lib_path/$licm2_so -no-aa -tbaa -targetlibinfo -basicaa -notti -verify -simplifycfg -domtree -sroa -lower-expect -targetlibinfo -no-aa -tbaa -basicaa -notti -verify -ipsccp -globalopt -deadargelim -range-based-aa -aa-eval $name.rbc -o $name.opt0.rbc


sage-opt -instcombine $name.opt0.rbc -o $name.opt1.rbc
sage-opt -load $lib_path/$ssify_so -break-crit-edges -ssify -set 1000  $name.opt1.rbc -o $name.opt1.rbc

sage-opt -stats -load $lib_path/$ssify_so -load $lib_path/$python_so -load $lib_path/$sage_so -load $lib_path/$sra_so -load $lib_path/$rbaa_so -load $lib_path/$licm2_so -range-based-pa -print-module -range-based-aa -aa-eval -licm2 -simplifycfg -basiccg -prune-eh -inline-cost -inline -functionattrs -argpromotion -sroa -domtree -lazy-value-info -jump-threading -correlated-propagation -simplifycfg -instcombine -tailcallelim -simplifycfg -reassociate -domtree -loops -loop-simplify -lcssa -loop-rotate -ssify -set 1000 $name.opt1.rbc -o $name.opt2.rbc


sage-opt -stats -load $lib_path/$python_so -load $lib_path/$sage_so -load $lib_path/$sra_so -load $lib_path/$rbaa_so -load $lib_path/$licm2_so -range-based-aa -aa-eval -licm2 -loop-unswitch -instcombine -scalar-evolution -lcssa -indvars -loop-idiom -loop-deletion -loop-unroll -memdep -mldst-motion -domtree -memdep -gvn -memdep -memcpyopt -sccp -instcombine -lazy-value-info -jump-threading -correlated-propagation -domtree -memdep -dse -loops -scalar-evolution -slp-vectorizer -adce -simplifycfg -instcombine -barrier -domtree -loops -loop-simplify -lcssa -branch-prob -block-freq -scalar-evolution -loop-vectorize -instcombine -simplifycfg -domtree -loops -loop-simplify -lcssa -scalar-evolution -loop-unroll -strip-dead-prototypes -globaldce -constmerge -verify -verify $name.opt2.rbc -o $name.opt.rbc 
llc $name.opt.rbc -o $name.s
clang $name.s -o $name.exe

#cleanup
rm $name.bc
#rm *.dot




