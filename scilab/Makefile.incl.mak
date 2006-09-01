#----------------------------------------------
# version for Visual C++
#----------------------------------------------

all::

MAKE=nmake /f Makefile.mak

#----------------------------------------------
# Scilab tclsci library 
#----------------------------------------------
# SCIDIR1 is set to . in Makefile.mak for compilation 
# and to scilab full path when used after compilation 
# for dynamic linking
USE_MT=-MT 
TCLTK=$(SCIDIR1)\modules\tclsci\includes\tcltk
TCLSCI=libs/tclsci.lib 
TKLIBS="$(SCIDIR1)\bin\tcl84.lib" "$(SCIDIR1)\bin\tk84.lib"
TKLIBSBIN=$(TKLIBS)
TCL_INCLUDES=-I"$(TCLTK)\include" -I"$(TCLTK)\include\X11"
#----------------------------------------------
# C compiler
# typically, for compiling use: CFLAGS = $(CC_OPTIONS)
# and for linking: $(CC) -o $(PROGRAM) $(OBJS) $(CC_LDFLAGS)
#----------------------------------------------
CC=cl
LINKER=link

# standard option for the linker 
LINKER_FLAGS=/NOLOGO /machine:ix86 /RELEASE

# debug for the linker 
#LINKER_FLAGS=/NOLOGO /machine:ix86 /DEBUG


# include options 
INCLUDES=-I"$(SCIDIR)/libs/f2c" $(TCL_INCLUDES) -I"$(SCIDIR)/modules/mexlib/includes"

CC_COMMON=-D__MSC__ -DWIN32 -c -DSTRICT -D_CRT_SECURE_NO_DEPRECATE -D__MAKEFILEVC__ -nologo $(INCLUDES) $(DTK) $(DPVM) $(DMKL) $(USE_MT)

# standard option for C compiler for VC 2005
CC_OPTIONS = $(CC_COMMON) -Z7 -W3 -O2 -Gd

# debug option for C compiler  for VC 2005
#CC_OPTIONS = $(CC_COMMON) -Zi -W3 -Od -Gd

CC_LDFLAGS = 
#----------------------------------------------
# Fortran Compiler 
# default usage is to use f2c 
#----------------------------------------------
USE_F2C=YES
# if USE_F2C is set to NO we will use the following Fortran compiler (i.e Visual Fortran)
!IF "$(USE_F2C)" == "NO"
FC=df 
FC_OPTIONS=/debug /nologo /assume:underscore /compile_only /iface:(cref,nomixed_str_len_arg) /names:lowercase 
LINKER_FLAGS=$(LINKER_FLAGS) /force:multiple
!ENDIF
#----------------------------------------------
# resource compiler 
#----------------------------------------------
RC=rc
RCVARS=-r -DWIN32
#----------------------------------------------
# Libraries 
#----------------------------------------------
GUIFLAGS=-SUBSYSTEM:console
GUI=comctl32.lib wsock32.lib shell32.lib winspool.lib user32.lib gdi32.lib comdlg32.lib kernel32.lib advapi32.lib 
GUILIBS=-NODEFAULTLIB:libc.lib -NODEFAULTLIB:msvcrt.lib $(GUI) libcmt.lib oldnames.lib
# XLIBS is used for linking Scilab
XLIBS=$(TKLIBS) $(PVMLIB) $(GUILIBS)
# XLIBSBIN is used by the binary version of Scilab for linking examples
XLIBSBIN=$(TKLIBSBIN) $(PVMLIB) $(GUILIBS) "$(SCIDIR1)\bin\atlas.lib" "$(SCIDIR1)\bin\libf2c.lib" "$(SCIDIR1)\bin\lapack.lib" "$(SCIDIR1)\bin\arpack.lib"
.c.obj	:
	@echo ------------- Compile file $< --------------
	$(CC) $(CFLAGS) $< 

.cpp.obj	:
	@echo ------------- Compile file $< --------------
	@$(CC) $(CFLAGS) $< 

# default rule for Fortran Compilation 

!IF "$(USE_F2C)" == "YES"
.f.obj	:
	@echo ----------- Compile file $*.f (using f2c) -------------
	@"$(SCIDIR1)\bin\f2c.exe" -I"$(SCIDIR1)\modules\core\includes" $(FFLAGS) $*.f 
	@$(CC) $(CFLAGS) $*.c 
	-del $*.c 
!ELSE 
.f.obj	:
	@echo -----------Compile file $*.f  (using $(FC)) -------------
	@$(FC) $(FFLAGS) $<
!ENDIF 
#----------------------------------------------
# clean 
RM = del
#----------------------------------------------
clean::
	-del *.bak 
  -del *.obj
distclean::  
  -del *.bak 
  -del *.obj
