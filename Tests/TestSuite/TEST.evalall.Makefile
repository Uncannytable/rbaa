##===- TEST.evalbasic.Makefile -----------------------------*- Makefile -*-===##
#
# Usage: 
#     make TEST=evalbasic (detailed list with time passes, etc.)
#     make TEST=evalbasic report
#     make TEST=evalbasic report.html
#
##===----------------------------------------------------------------------===##

CURDIR  := $(shell cd .; pwd)
PROGDIR := $(PROJ_SRC_ROOT)
RELDIR  := $(subst $(PROGDIR),,$(CURDIR))

#LLVM_DIR = "/home/vitor/Ecosoc"
#LLVM_BUILD = "Release+Asserts"


$(PROGRAMS_TO_TEST:%=test.$(TEST).%): \
test.$(TEST).%: Output/%.$(TEST).report.txt
	@cat $<

$(PROGRAMS_TO_TEST:%=Output/%.$(TEST).report.txt):  \
Output/%.$(TEST).report.txt: Output/%.linked.rbc $(LOPT) \
	$(PROJ_SRC_ROOT)/TEST.evalbasic.Makefile 
	$(VERB) $(RM) -f $@
	@echo "---------------------------------------------------------------" >> $@
	@echo ">>> ========= '$(RELDIR)/$*' Program" >> $@
	@echo "---------------------------------------------------------------" >> $@
	@~/Ecosoc/llvm-3.5.0/lib/Ecosoc/llvm-sra/SAGE/bin/sage-opt -load SSIfy.so -mem2reg -instnamer -break-crit-edges -ssify -set 1000  $< -o $<.ssify.bc 2>>$@
	@~/Ecosoc/llvm-3.5.0/lib/Ecosoc/llvm-sra/SAGE/bin/sage-opt -load Python.so -load SAGE.so -load SRA.so -load RangeBasedAliasAnalysis.so -redef -basicaa -libcall-aa -objc-arc-aa -scev-aa -tbaa -aa-eval $<.ssify.bc -o $<.redef.bc 2>>$@ 

REPORT_DEPENDENCIES := ~/Ecosoc/llvm-3.5.0/lib/Ecosoc/llvm-sra/SAGE/bin/sage-opt
