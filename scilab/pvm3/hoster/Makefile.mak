#
# $Id: Makefile.mak,v 1.1 2002/10/28 13:56:34 chanceli Exp $
#

#*************************************************************#
#**                                                         **#
#**      Nmake file for the PVM hoster                      **#
#**                   hoster.exe                            **#
#**                                                         **#
#**                                                         **#
#*************************************************************#


!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

SDIR	=	$(PVM_ROOT)\src
HDIR	=	$(PVM_ROOT)\hoster

OBJS	=	$(PVM_ARCH)\hoster.obj $(PVM_ARCH)\pvmwinrexec.obj \
			$(PVM_ARCH)\pvmwinrsh.obj

all:	paths hoster

#

paths:
	@if not exist "$(PVM_ROOT)\bin\$(PVM_ARCH)\$(NULL)" mkdir "$(PVM_ROOT)\bin\$(PVM_ARCH)"
	@if not exist "$(PVM_ARCH)\$(NULL)" mkdir "$(PVM_ARCH)"

hoster: $(PVM_ROOT)\bin\$(PVM_ARCH)\hoster.exe 

$(PVM_ROOT)\bin\$(PVM_ARCH)\hoster.exe :   $(OBJS)
	@echo Creation of $*.exe
	@$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\hoster.exe \
		$(OBJS) $(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib \
		$(link_multithread) /nodefaultlib:libc.lib

$(PVM_ARCH)\hoster.obj:  $(HDIR)\hoster.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\hoster.obj $(HDIR)\hoster.c

$(PVM_ARCH)\pvmwinrexec.obj:  $(HDIR)\pvmwinrexec.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\pvmwinrexec.obj \
		$(HDIR)\pvmwinrexec.c

$(PVM_ARCH)\pvmwinrsh.obj:  $(HDIR)\pvmwinrsh.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\pvmwinrsh.obj \
		$(HDIR)\pvmwinrsh.c

# Clean up everything but the .EXEs

clean :
	-del *.obj
	-del *.pdb

