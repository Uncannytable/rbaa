#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b01/b01.redef.bc -o /dev/null 2>>b01.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b02/b02.redef.bc -o /dev/null 2>>b02.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b03/b03.redef.bc -o /dev/null 2>>b03.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b04/b04.redef.bc -o /dev/null 2>>b04.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b05/b05.redef.bc -o /dev/null 2>>b05.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b06/b06.redef.bc -o /dev/null 2>>b06.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b07/b07.redef.bc -o /dev/null 2>>b07.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b08/b08.redef.bc -o /dev/null 2>>b08.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b09/b09.redef.bc -o /dev/null 2>>b09.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b10/b10.redef.bc -o /dev/null 2>>b10.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b11/b11.redef.bc -o /dev/null 2>>b11.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -gvn -dse -instcount -stats b12/b12.redef.bc -o /dev/null 2>>b12.txt
