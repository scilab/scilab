#
# $Id: Makefile,v 1.1 2001/04/26 07:47:14 scilab Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	SP2MPI
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-mpp 

install:	
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-mpp

clean:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

