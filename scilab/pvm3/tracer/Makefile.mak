#*************************************************************#
#
#  $Id: Makefile.mak,v 1.2 2002/10/24 16:29:06 chanceli Exp $
#
#  Nmake file for the PVM tracer
#
#  PVM_ARCH   = the official pvm-name of your processor
#  ARCHCFLAGS = special cc flags
#

#  USER installation specific part -- to be modified

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

INCL	=	/I $(PVM_ROOT)\include /I $(PVM_ROOT)\tracer \
			/I $(PVM_ROOT)\src

LIBS	=	$(PVM_ARCH)\libpvmtracer.lib \
			$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib

OBJS	=	$(PVM_ARCH)\cmd.obj $(PVM_ARCH)\trcglob.obj $(PVM_ARCH)\tracer.obj

LIBOBJS	=	$(PVM_ARCH)\trcmess.obj $(PVM_ARCH)\trcfile.obj \
		$(PVM_ARCH)\trccompat.obj $(PVM_ARCH)\trccompatglob.obj \
		$(PVM_ARCH)\trcutil.obj $(PVM_ARCH)\trclib.obj

all:	paths tracer_lib tracer_exe

paths:
	@if not exist "$(PVM_ARCH)\$(NULL)" mkdir "$(PVM_ARCH)"
	@if not exist "..\lib\$(PVM_ARCH)\$(NULL)" mkdir "..\lib\$(PVM_ARCH)"

tracer_lib: $(PVM_ROOT)\tracer\$(PVM_ARCH)\libpvmtracer.lib 

$(PVM_ROOT)\tracer\$(PVM_ARCH)\libpvmtracer.lib :  $(LIBOBJS)
	@echo Creation of $*.lib
	@$(linklib) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\tracer\$(PVM_ARCH)\libpvmtracer.lib \
		$(LIBOBJS)

tracer_exe: $(PVM_ROOT)\lib\$(PVM_ARCH)\tracer.exe

$(PVM_ROOT)\lib\$(PVM_ARCH)\tracer.exe : $(OBJS) tracer_lib
	@echo Creation of $*.exe
	@$(linkexe) $(OUTBIN)$(PVM_ROOT)\lib\$(PVM_ARCH)\tracer.exe \
		$(OBJS) $(LIBS) $(link_flags)
	@echo Creation of $*.exe done

$(PVM_ARCH)\cmd.obj: cmd.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\cmd.obj  cmd.c

$(PVM_ARCH)\trcglob.obj: trcglob.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trcglob.obj trcglob.c

$(PVM_ARCH)\tracer.obj: tracer.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\tracer.obj tracer.c

# LIB

$(PVM_ARCH)\trcmess.obj: trcmess.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trcmess.obj trcmess.c

$(PVM_ARCH)\trcfile.obj: trcfile.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trcfile.obj trcfile.c

$(PVM_ARCH)\trccompat.obj: trccompat.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trccompat.obj \
		trccompat.c

$(PVM_ARCH)\trccompatglob.obj: trccompatglob.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trccompatglob.obj \
		trccompatglob.c

$(PVM_ARCH)\trcutil.obj: trcutil.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trcutil.obj trcutil.c

$(PVM_ARCH)\trclib.obj: trclib.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\trclib.obj trclib.c

