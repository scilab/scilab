SHELL = /bin/sh

SCIDIR=..
include ../Makefile.incl.mak

# just add -DEPSFIG 
# il you prefer epsfig to special to insert postscript files in LaTeX 

CFLAGS = $(CC_OPTIONS)

all:: $(SCIDIR)/bin/Blpr.exe $(SCIDIR)/bin/Blatexprs.exe \
	$(SCIDIR)/bin/Blatexpr2.exe \
	$(SCIDIR)/util/Blatdocs.exe $(SCIDIR)/bin/BEpsf.exe \
	 $(SCIDIR)/bin/Blatexpr.exe \
	 $(SCIDIR)/util/Blatdoc.exe

$(SCIDIR)/bin/Blpr.exe: Slpr.obj env.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" Slpr.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\bin\Blpr.exe

$(SCIDIR)/bin/BEpsf.exe: SEpsf.obj env.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" SEpsf.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\bin\BEpsf.exe

$(SCIDIR)/bin/Blatexpr.exe: Slatexpr.obj env.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" Slatexpr.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\bin\Blatexpr.exe

$(SCIDIR)/bin/Blatexprs.exe: Slatexprs.obj env.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" Slatexprs.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\bin\Blatexprs.exe

$(SCIDIR)/bin/Blatexpr2.exe: Slatexpr2.obj env.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" Slatexpr2.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\bin\Blatexpr2.exe

$(SCIDIR)/util/Blatdocs.exe: Slatdocs.obj env.obj
	$(CC) $(CFLAGS) -DDOC Slatexprs.c
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" Slatdocs.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\util\Blatdocs.exe 

$(SCIDIR)/util/Blatdoc.exe: Slatdoc.obj env.obj
	@$(LINKER) -SUBSYSTEM:console -OUT:"$@" Slatdoc.obj env.obj $(GUILIBS)

distclean::
	@del $(SCIDIR)\util\Blatdoc.exe

clean::
	@del *.obj 

distclean:: clean

