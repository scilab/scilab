#-----------------------------
# generated from Makefile: DO NOT EDIT
# -----------------------------
SHELL = /bin/sh

# SCIDIR must be bound to your running Scilab directory 
# SCIDIR1 also for msvc Makfile 
SCIDIR=../..
SCIDIR1=..\..

include  ../../Makefile.incl.mak

.SUFFIXES: .sci .bin $(SUFFIXES)
# NAME is the name of the created library
# all the functions in the library are loaded into Scilab by the command
# "load mylibrary"
NAME = mylibrary

# NAM is the pathname of the directory containing the .sci files. 
NAM = $(SCIDIR)/examples/macros-examples

# Put here the list of the functions to be inserted in the library
MACROS = f.sci g.sci 

include  ../../macros/Make.lib.mak

tests	:: all

