include ../Path.incl

#
# $Id: Makefile,v 1.2 2002/10/14 14:46:32 chanceli Exp $
#
# top-level PVM3 makefile
#

SHELL	=	/bin/sh

AIMKSTR	=	-here -f ./conf/`./lib/pvmtmparch`.def -f ./Makefile.aimk

all: default 

default:
	@PVM_ROOT=$(SCIDIR)/pvm3; export PVM_ROOT; ./lib/aimk $(AIMKSTR) default

clean:
	@PVM_ROOT=$(SCIDIR)/pvm3; export PVM_ROOT; ./lib/aimk $(AIMKSTR) clean

# distclean clean removes *.o and other remaining executables 

distclean:: clean 
	arch=`./lib/pvmtmparch` ;\
	$(RM) -rf lib/$$arch bin/$$arch pvmgs/$$arch  

ALL_ARCH = HPPA SGI ALPHA LINUXALPHA LINUX LINUXSPARC SUN4SOL2 SUNMP WIN32 MINGW

# make a distclean on one architecture and removes binaries for other 
# architectures 

super_distclean:
	@for arch in $(ALL_ARCH) ;\
        do \
	( aimkop="-here -f ./conf/$$arch.def -f ./Makefile.aimk" ;\
	 PVM_ROOT=`pwd` ; export PVM_ROOT; ./lib/aimk $$aimkop clean ; \
	 $(RM) -rf lib/$$arch bin/$$arch pvmgs/$$arch  \
            ); \
        done
