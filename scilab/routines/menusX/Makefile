# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..

LIBRARY = $(SCIDIR)/libs/menusX.a

COMM = men_message.o men_choose.o men_dialog.o men_mdial.o  \
	men_madial.o men_choice.o \
	men_print.o men_getfile.o 


XOBJS =	xmen_choose.o xmen_Utils.o \
	xmen_dialog.o xmen_choice.o xmen_getfile.o xmen_mdial.o \
	xmen_madial.o xmen_message.o xmen_print.o

WOBJS = wmen_choice.o wmen_choose.o wmen_dialog.o wmen_getfile.o \
	wmen_mdial.o wmen_message.o wmen_print.o wmen_madial.o

OBJSC = $(COMM) $(XOBJS) 

include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

include ../Make.lib

#==============dependencies 

$(OBJSC) :  ../machine.h men_scilab.h wmen_scilab.h 
men_print.o xmen_choice.o wmen_choice.o men_choice.o : men_choice.h
