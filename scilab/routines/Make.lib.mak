# CFLAGS IS CHANGED LOCALLY FOR EACH SUBDIR 
# CFLAGS = $(CC_OPTIONS) 
# or 
# CFLAGS = $(CC_OPTIONS) $(XFLAGS)

FFLAGS = $(FC_OPTIONS)

OBJS = $(OBJSC) $(OBJSF)

all:: $(LIBRARY)

# standard library 

$(LIBRARY): $(OBJS)
	@echo Creation of $(LIBRARY)
	@$(LINKER) -lib $(OBJS) /nologo /out:"$@" 
	@echo Done

# A partial def file (without headers) for the library 
# to build a scilex.def 

DEF=$(LIBRARY:.lib=.def)

all:: $(DEF)

$(DEF) : $(OBJS) 
	@echo Creation of $(DEF)
	@$(SCIDIR1)\bin\dumpexts.exe -o $(DEF) -n scilex.dll $(OBJS)

# Not used up to now 

DLL=$(LIBRARY:.lib=.dll)

dll:: $(DLL)

$(DLL) : $(OBJS) 
	@echo Creation of dll $(DLL) and import lib 
	$(LINKER) $(OBJS) $(DLL_LIBS) $(GUILIBS) /dll /out:"$*.dll" /implib:"$*.ilib" $(LINKER_FLAGS) /MAP:"$*.map"  /def:"$*.def"

# /PDB:NONE

clean:: cleanC cleanF 

cleanC :
	-del *.obj

cleanF :
	-del *.obj

distclean::
	-del *.obj


# won't work since LIBRARY is a path with \ del $(LIBRARY)

