#
# $Id: Makefile,v 1.1 2002/10/14 14:40:04 chanceli Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/src

PVM_ARCH	=	BEOLIN
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)


all:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk all-beo 

install:	
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-beo

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

