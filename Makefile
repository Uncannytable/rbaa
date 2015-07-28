##===--------------------- Makefile ------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

# Makefile for RangeBasedAnalysis passes

# Path to top level of LLVM hierarchy
LEVEL = ../../..

# Name of the library to build
LIBRARYNAME = RangeBasedAliasAnalysis

# Make the shared library become a loadable module so the tools can 
# dlopen/dlsym on the resulting library.
LOADABLE_MODULE = 1

CXXFLAGS += -std=c++0x -fpermissive 

# Include the makefile implementation stuff
include $(LEVEL)/Makefile.common
