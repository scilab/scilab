#
# $Id: Makefile,v 1.1 2001/04/26 07:47:09 scilab Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/pvmgs

PVM_ARCH	=	PGON
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)
		
default:    
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install-mpp

clean:
	make -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean
	
$(LIBDIR):
	mkdir $(LIBDIR)

