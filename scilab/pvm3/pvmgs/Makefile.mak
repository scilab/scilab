#
# $Id: Makefile.mak,v 1.3 2002/10/24 16:29:05 chanceli Exp $
#

#*************************************************************#
#**                                                         **#
#**      (N)make file for                                   **#
#**            libgpvm3.lib pvmgs                           **#
#**                                                         **#
#**                                                         **#
#**                                                         **#
#*************************************************************#

#  USER installation specific part -- to be modified

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

!include $(PVM_ROOT)\conf\$(PVM_ARCH).def

SDIR	=	$(PVM_ROOT)\pvmgs

LOBJ	=	$(PVM_ARCH)\pvmgsu_aux.obj \
			$(PVM_ARCH)\pvmgsu_core.obj \
			$(PVM_ARCH)\pvmgs_func.obj

#all:  paths libgpvm3.lib $(libgpvm3dll) pvmgs.exe
all:  paths libgpvm3.lib pvmgs.exe

paths:
	@if not exist "$(PVM_ARCH)\$(NULL)" mkdir "$(PVM_ARCH)"
	@if not exist "..\lib\$(PVM_ARCH)\$(NULL)" mkdir "..\lib\$(PVM_ARCH)"
	@if not exist "..\bin\$(PVM_ARCH)\$(NULL)" mkdir "..\bin\$(PVM_ARCH)"

pvmgs.exe:	$(PVM_ROOT)\bin\$(PVM_ARCH)\pvmgs.exe 

$(PVM_ROOT)\bin\$(PVM_ARCH)\pvmgs.exe :  $(PVM_ARCH)\pvmgs_core.obj $(PVM_ARCH)\pvmgs_func.obj
	@echo Creation of $*.exe
	@$(linkexe) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\bin\$(PVM_ARCH)\pvmgs.exe \
		$(PVM_ARCH)\pvmgs_core.obj $(PVM_ARCH)\pvmgs_func.obj \
		$(PVM_ROOT)\lib\$(PVM_ARCH)\libpvm3.lib $(link_flags)

libgpvm3.lib: $(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.lib 

$(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.lib : $(LOBJ)
	@echo Creation of $*.lib
	@$(linklib) $(conflags) \
		$(OUTBIN)$(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.lib $(LOBJ)

libgpvm3.dll: $(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.dll

$(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.dll : $(LOBJ)
	@echo Creation of $*.dll
	@$(linkdll) $(conflags) \
		$(OUTDLL)$(PVM_ROOT)\lib\$(PVM_ARCH)\libgpvm3.dll $(LOBJ) \
		$(link_flags)

$(PVM_ARCH)\pvmgsu_aux.obj:  $(SDIR)\pvmgsu_aux.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\pvmgsu_aux.obj $(SDIR)\pvmgsu_aux.c
$(PVM_ARCH)\pvmgsu_core.obj: $(SDIR)\pvmgsu_core.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\pvmgsu_core.obj $(SDIR)\pvmgsu_core.c
$(PVM_ARCH)\pvmgs_func.obj: $(SDIR)\pvmgs_func.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\pvmgs_func.obj $(SDIR)\pvmgs_func.c
$(PVM_ARCH)\pvmgs_core.obj: $(SDIR)\pvmgs_core.c
	$(cc) $(cdebug) $(cflags) $(cvars) \
		$(OUT)$(PVM_ARCH)\pvmgs_core.obj $(SDIR)\pvmgs_core.c

# Clean up everything but the .EXEs
clean :
	-del $(PVM_ARCH)\*.obj

