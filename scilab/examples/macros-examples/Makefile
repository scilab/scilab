SHELL = /bin/sh

# SCIDIR must be bound to your running Scilab directory 
# SCIDIR1 also for msvc Makfile 
SCIDIR=../..
SCIDIR1=..\..

include  $(SCIDIR)/Makefile.incl

.SUFFIXES: .sci .bin $(SUFFIXES)
# NAME is the name of the created library
# all the functions in the library are loaded into Scilab by the command
# "load mylibrary"
NAME = mylibrary

# NAM is the pathname of the directory containing the .sci files. 
NAM = `pwd`

# Put here the list of the functions to be inserted in the library
MACROS = f.sci g.sci 

include  $(SCIDIR)/macros/Make.lib

tests	:: all

