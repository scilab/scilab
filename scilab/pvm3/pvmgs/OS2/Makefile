#
# $Id: Makefile,v 1.2 2002/10/14 14:27:43 chanceli Exp $
#

PVMDIR		=	../..
SDIR		=	$(PVMDIR)/pvmgs

PVM_ARCH	=	OS2
CONFFILE	=	$(PVMDIR)/conf/$(PVM_ARCH).def
LIBDIR		=	$(PVMDIR)/lib/$(PVM_ARCH)

#
install:	default $(LIBDIR)
		
default:    
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk install

clean:
	make LIBPREFIX="" -f $(CONFFILE) -f $(SDIR)/Makefile.aimk clean
	
$(LIBDIR):
	mkdir $(LIBDIR)

