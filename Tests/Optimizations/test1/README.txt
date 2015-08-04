28 de Julho de 2015

Dear Vitor,

   I am sending you an example that you should be able to optimize
with your range analysis. Please, focus on it. I am explaining you how
to test it below. First, consider the file vitorOpt1.c below:

#include <stdio.h>
#include <stdlib.h>
int no_body_sum(int* x, int* y);
int main(int argc, char** argv) {
  int* v = (int*)malloc(6 * sizeof(int));
  int a = 0, b = 1;
  v[0] = 13;
  v[1] = 17;
  v[a] += v[b];
  v[a] += v[b];
  no_body_sum(v, v + 1);
}

Notice the function without body. This is to ensure that LLVM will not
throw code away. Try to compile it with the following command line:

$> clang -c -emit-llvm vitorOpt1.c -o vitorOpt1.bc

$> opt -mem2reg -instnamer -targetlibinfo -no-aa -tbaa -scoped-noalias
-assumption-tracker -basicaa -notti -ipsccp -globalopt -deadargelim
-domtree -instcombine -simplifycfg -basiccg -prune-eh -inline-cost
-inline -functionattrs -argpromotion -sroa -domtree -early-cse
-lazy-value-info -jump-threading -correlated-propagation -simplifycfg
-domtree -instcombine -tailcallelim -simplifycfg -reassociate -domtree
-loops -loop-simplify -lcssa -loop-rotate -licm -loop-unswitch
-instcombine -scalar-evolution -loop-simplify -lcssa -indvars
-loop-idiom -loop-deletion -function_tti -loop-unroll -memdep
-mldst-motion -domtree -memdep -gvn -memdep -memcpyopt -sccp -domtree
-instcombine -lazy-value-info -jump-threading -correlated-propagation
-domtree -memdep -dse -adce -simplifycfg -domtree -instcombine
-barrier -domtree -loops -loop-simplify -lcssa -branch-prob
-block-freq -scalar-evolution -loop-vectorize -instcombine
-scalar-evolution -slp-vectorizer -simplifycfg -domtree -instcombine
-loops -loop-simplify -lcssa -scalar-evolution -function_tti
-loop-unroll -alignment-from-assumptions -strip-dead-prototypes
-globaldce -constmerge -verify vitorOpt1.bc -o vitorOpt1.rbc

$> llvm-dis < vitorOpt1.rbc # This is just to see the assembly

You will find the following program:

define i32 @main(i32 %argc, i8** nocapture readnone %argv) #0 {
entry:
  %call = tail call i8* @malloc(i64 24) #3
  %tmp = bitcast i8* %call to i32*
  %arrayidx1 = getelementptr inbounds i8* %call, i64 4
  %0 = bitcast i8* %arrayidx1 to i32*
  store i32 17, i32* %0, align 4
  store i32 47, i32* %tmp, align 4
  %call10 = tail call i32 @no_body_sum(i32* %tmp, i32* %0) #3
  ret i32 0
}

Notice that LLVM has been able to constant-fold the second addition.
You only have two stores into 'v' before calling no_body_sum. Now,
let's remove the alias analysis from the command line, e.g.,
(-basicaa):

$> opt -mem2reg -instnamer -targetlibinfo -no-aa -tbaa -scoped-noalias
-assumption-tracker -notti -ipsccp -globalopt -deadargelim -domtree
-instcombine -simplifycfg -basiccg -prune-eh -inline-cost -inline
-functionattrs -argpromotion -sroa -domtree -early-cse
-lazy-value-info -jump-threading -correlated-propagation -simplifycfg
-domtree -instcombine -tailcallelim -simplifycfg -reassociate -domtree
-loops -loop-simplify -lcssa -loop-rotate -licm -loop-unswitch
-instcombine -scalar-evolution -loop-simplify -lcssa -indvars
-loop-idiom -loop-deletion -function_tti -loop-unroll -memdep
-mldst-motion -domtree -memdep -gvn -memdep -memcpyopt -sccp -domtree
-instcombine -lazy-value-info -jump-threading -correlated-propagation
-domtree -memdep -dse -adce -simplifycfg -domtree -instcombine
-barrier -domtree -loops -loop-simplify -lcssa -branch-prob
-block-freq -scalar-evolution -loop-vectorize -instcombine
-scalar-evolution -slp-vectorizer -simplifycfg -domtree -instcombine
-loops -loop-simplify -lcssa -scalar-evolution -function_tti
-loop-unroll -alignment-from-assumptions -strip-dead-prototypes
-globaldce -constmerge -verify vitorOpt1.bc -o vitorOpt1.rbc

$> llvm-dis < vitorOpt1.rbc

You will find the following program:

define i32 @main(i32 %argc, i8** nocapture readnone %argv) #0 {
entry:
  %call = tail call i8* @malloc(i64 24) #3
  %tmp = bitcast i8* %call to i32*
  store i32 13, i32* %tmp, align 4
  %arrayidx1 = getelementptr inbounds i8* %call, i64 4
  %0 = bitcast i8* %arrayidx1 to i32*
  store i32 17, i32* %0, align 4
  %tmp2 = load i32* %tmp, align 4
  %add = add nsw i32 %tmp2, 17
  store i32 %add, i32* %tmp, align 4
  %tmp3 = load i32* %0, align 4
  %add9 = add nsw i32 %add, %tmp3
  store i32 %add9, i32* %tmp, align 4
  %call10 = tail call i32 @no_body_sum(i32* %tmp, i32* %0) #3
  ret i32 0
}

Ok, now, let's try a program that your analysis should be able to
optimize, but that LLVM cannot handle at this point:

#include <stdio.h>
#include <stdlib.h>
int no_body_sum(int* x, int* y);
int main(int argc, char** argv) {
  int* v = (int*)malloc(6 * sizeof(int));
  int a, b;
  v[0] = argc;
  v[1] = argc;
  switch(argc % 3) {
    case 0:  a = 0; b = 3; break;
    case 1:  a = 1; b = 4; break;
    default: a = 2; b = 5;
  }
  v[a] += v[b];
  v[a] += v[b];
  no_body_sum(v, v + 1);
}

So, our immediate goal is to enable optimizations for this program. At
this point, LLVM cannot optimize it, neither if we enable all the
alias analyses that it has now.

Regards,

Fernando




Passes that optimize these examples:
-gvn -dse 
