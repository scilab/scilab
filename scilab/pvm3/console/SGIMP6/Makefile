#
# $Id: Makefile,v 1.1 2002/10/14 14:15:54 chanceli Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/console

PVM_ARCH	=	SGIMP6
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)

default:
	$(MAKE) PVMLIB=pvm3s -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	$(MAKE) -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean

$(LIBDIR):
	mkdir $(LIBDIR)

