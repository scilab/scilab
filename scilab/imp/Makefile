SHELL = /bin/sh

SCIDIR=..
include ../Makefile.incl

# just add -DEPSFIG 
# il you prefer epsfig to special to insert postscript files in LaTeX 

CFLAGS = $(CC_OPTIONS)

all:: $(SCIDIR)/bin/Slpr $(SCIDIR)/bin/Slatexprs $(SCIDIR)/bin/Slatexpr2 \
	$(SCIDIR)/util/Slatdocs $(SCIDIR)/bin/SEpsf $(SCIDIR)/bin/Slatexpr \
	 $(SCIDIR)/util/Slatdoc

$(SCIDIR)/bin/Slpr: Slpr.o
	$(RM) $@
	$(CC) -o $@ Slpr.o $(CC_LDFLAGS)
clean::
	$(RM) Slpr.o
distclean::
	$(RM) Slpr.o $(SCIDIR)/bin/Slpr

$(SCIDIR)/bin/SEpsf: SEpsf.o
	$(RM) $@
	$(CC) -o $@ SEpsf.o $(CC_LDFLAGS)
clean::
	$(RM) SEpsf.o
distclean::
	$(RM) SEpsf.o $(SCIDIR)/bin/SEpsf

$(SCIDIR)/bin/Slatexpr: Slatexpr.o
	$(RM) $@
	$(CC) -o $@ Slatexpr.o $(CC_LDFLAGS)
clean::
	$(RM) Slatexpr.o
distclean::
	$(RM) Slatexpr.o $(SCIDIR)/bin/Slatexpr

$(SCIDIR)/bin/Slatexprs: Slatexprs.o
	$(RM) $@
	$(CC) -o $@ Slatexprs.o $(CC_LDFLAGS)
clean::
	$(RM) Slatexprs.o
distclean::
	$(RM) Slatexprs.o $(SCIDIR)/bin/Slatexprs

$(SCIDIR)/bin/Slatexpr2: Slatexpr2.o
	$(RM) $@
	$(CC) -o $@ Slatexpr2.o $(CC_LDFLAGS)
clean::
	$(RM) Slatexpr2.o
distclean::
	$(RM) Slatexpr2.o $(SCIDIR)/bin/Slatexpr2

$(SCIDIR)/util/Slatdocs: Slatdocs.o
	$(RM) $@
	$(CC) -o $@ Slatdocs.o   $(CC_LDFLAGS)

clean::
	$(RM) Slatdocs.o
distclean::
	$(RM) Slatdocs.o $(SCIDIR)/util/Slatdocs

$(SCIDIR)/util/Slatdoc: Slatdoc.o
	$(RM) $@
	$(CC) -o $@ Slatdoc.o  $(CC_LDFLAGS)


clean::
	$(RM) Slatdoc.o
distclean::
	$(RM) Slatdoc.o $(SCIDIR)/util/Slatdoc


