SCIDIR =../../..
SCIDIR1 =..\..\..
# name of the dll to be built
LIBRARY = libtst 
# list of objects file
OBJS = libtst.obj
# added libraries 
OTHERLIBS = xtimesy.lib
!include $(SCIDIR)/Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines" 
FFLAGS = $(FC_OPTIONS) -DFORDLL -I"$(SCIDIR)/routines"
EXTRA_LDFLAGS = 
!include $(SCIDIR1)\config\Makedll.incl 
