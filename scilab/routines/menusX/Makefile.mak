# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh
SCIDIR=../..
SCIDIR1=..\..

LIBRARY = $(SCIDIR)/libs/menusX.lib

COMM = men_message.obj men_choose.obj men_dialog.obj men_mdial.obj  \
	men_madial.obj men_choice.obj \
	men_print.obj men_getfile.obj 


XOBJS =	xmen_choose.obj xmen_Utils.obj \
	xmen_dialog.obj xmen_choice.obj xmen_getfile.obj xmen_mdial.obj \
	xmen_madial.obj xmen_message.obj xmen_print.obj

WOBJS = wmen_choice.obj wmen_choose.obj wmen_dialog.obj wmen_getfile.obj \
	wmen_mdial.obj wmen_message.obj wmen_print.obj wmen_madial.obj

OBJSC = $(COMM) $(WOBJS) 

include ../../Makefile.incl.mak

CFLAGS = $(CC_OPTIONS) $(XFLAGS)

include ../Make.lib.mak

#==============dependencies 

$(OBJSC) :  ../machine.h men_scilab.h wmen_scilab.h 
men_print.obj xmen_choice.obj wmen_choice.obj men_choice.obj : men_choice.h
