SCIDIR =../../../..
SCIDIR1 =..\..\..\..
# name of the dll to be built
LIBRARY = libtst 
# list of objects file
OBJS = libtst.obj
# added libraries 
OTHERLIBS = xtimesy.lib
!include $(SCIDIR)/Makefile.incl.mak
CFLAGS = $(CC_OPTIONS) -DFORDLL -I"$(SCIDIR)\libs\MALLOC\includes" -I"$(SCIDIR)\modules\core\includes" -I"$(SCIDIR)/libs/f2c" -I"$(SCIDIR)/modules/mexlib/includes" 
FFLAGS = $(FC_OPTIONS) -DFORDLL -I"$(SCIDIR)\libs\MALLOC\includes" -I"$(SCIDIR)\modules\core\includes" -I"$(SCIDIR)/libs/f2c" -I"$(SCIDIR)/modules/mexlib/includes" 
EXTRA_LDFLAGS = 
!include $(SCIDIR1)\modules\dynamic_link\src\scripts\Makedll.incl 
