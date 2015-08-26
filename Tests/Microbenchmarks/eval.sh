#!/bin/bash
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b01/b01.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b02/b02.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b03/b03.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b04/b04.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b05/b05.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b06/b06.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b07/b07.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b08/b08.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b09/b09.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b10/b10.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b11/b11.redef.bc -o /dev/null 2>>evalrbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -range-based-aa -aa-eval -print-all-alias-modref-info -stats b12/b12.redef.bc -o /dev/null 2>>evalrbasic.txt

sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b01/b01.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b02/b02.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b03/b03.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b04/b04.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b05/b05.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b06/b06.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b07/b07.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b08/b08.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b09/b09.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b10/b10.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b11/b11.redef.bc -o /dev/null 2>>evalrbaa.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -range-based-aa -aa-eval -print-all-alias-modref-info -stats b12/b12.redef.bc -o /dev/null 2>>evalrbaa.txt

sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b01/b01.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b02/b02.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b03/b03.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b04/b04.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b05/b05.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b06/b06.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b07/b07.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b08/b08.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b09/b09.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b10/b10.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b11/b11.redef.bc -o /dev/null 2>>evalbasic.txt
sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -basicaa -aa-eval -print-all-alias-modref-info -stats b12/b12.redef.bc -o /dev/null 2>>evalbasic.txt
