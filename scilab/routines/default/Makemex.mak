SCIDIR =../..
SCIDIR1 =..\..
# name of the dll to be built
LIBRARY = mexlib
# list of objects file
OBJS = mexlib.obj
# added libraries 
OTHERLIBS = 
!include $(SCIDIR1)\Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines" -Dmexfunction_=mex$*_  -DmexFunction=mex_$*  
FFLAGS = $(FC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines" -Dmexfunction=mex$* 
EXTRA_LDFLAGS = 
!include $(SCIDIR1)\config\Makedll.incl 
