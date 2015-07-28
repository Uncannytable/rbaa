##======- lib/*/*/Makefile ------------------------------*- Makefile -*-======##
##===----------------------------------------------------------------------===##

LEVEL ?= ../../..

all:
	./../llvm-sra/SAGE/bin/sage -sh -c "make -f Makefile.llvm LEVEL=$(LEVEL)"

