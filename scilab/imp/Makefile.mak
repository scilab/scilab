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

$(SCIDIR)/bin/Blpr.exe: Slpr.obj util.obj env.obj
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" Slpr.obj util.obj env.obj $(GUILIBS)
$(SCIDIR)/bin/BEpsf.exe: SEpsf.obj util.obj env.obj
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" SEpsf.obj util.obj env.obj $(GUILIBS)
$(SCIDIR)/bin/Blatexpr.exe: Slatexpr.obj util.obj env.obj
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" Slatexpr.obj util.obj env.obj $(GUILIBS)
$(SCIDIR)/bin/Blatexprs.exe: Slatexprs.obj util.obj env.obj
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" Slatexprs.obj util.obj env.obj $(GUILIBS)
$(SCIDIR)/bin/Blatexpr2.exe: Slatexpr2.obj util.obj env.obj
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" Slatexpr2.obj util.obj env.obj $(GUILIBS)
$(SCIDIR)/util/Blatdocs.exe: Slatdocs.obj util.obj env.obj
	$(CC) $(CFLAGS) -DDOC Slatexprs.c
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" Slatdocs.obj util.obj env.obj $(GUILIBS)
$(SCIDIR)/util/Blatdoc.exe: Slatdoc.obj util.obj env.obj
	@$(LINKER) $(LINKER_FLAGS) -SUBSYSTEM:console -OUT:"$@" Slatdoc.obj util.obj env.obj $(GUILIBS)

clean::
	@del $(SCIDIR)\bin\Blpr.exe
	@del $(SCIDIR)\bin\BEpsf.exe
	@del $(SCIDIR)\bin\Blatexpr.exe
	@del $(SCIDIR)\bin\Blatexprs.exe
	@del $(SCIDIR)\bin\Blatexpr2.exe
	@del $(SCIDIR)\util\Blatdocs.exe
	@del $(SCIDIR)\util\Blatdoc.exe
	@del *.obj 

distclean:: 
	@del *.obj
