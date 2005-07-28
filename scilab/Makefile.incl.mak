#----------------------
# version for Visual C++
#------------------------

all::

MAKE=nmake /f Makefile.mak

#---------------------
# Scilab tclsci library 
#---------------------
# To compile with TCL/TK interface, uncomment the following lines and give
# the good pathnames for TKLIBS and TCL_INCLUDES.
# compiler flags: -MT or -MD  only needed if tcl/tk is used

!IF "$(DTK)" == ""
DTK=
!ELSE
DTK=-DWITH_TK
!ENDIF

!IF "$(DTK)" == "-DWITH_TK"
# -MT can be added here (note that DPVM=-DWITH_PVM will remove it)
USE_MT=-MT 
# SCIDIR1 is set to . in Makefile.mak for compilation 
# and to scilab full path when used after compilation 
# for dynamic linking
TCLTK=$(SCIDIR1)\tcl
TCLSCI=libs/tclsci.lib 
TKLIBS="$(SCIDIR1)\bin\tcl84.lib" "$(SCIDIR1)\bin\tk84.lib"
TKLIBSBIN=$(TKLIBS)
TCL_INCLUDES=-I"$(TCLTK)\include" -I"$(TCLTK)\include\X11"
!ENDIF

#---------------------
# Scilab pvm library
#---------------------
# To compile with PVM interface, uncomment the following lines and give
# the good pathname for PVM_ROOT.
#

!IF "$(DPVM)"==""
DPVM=
!ELSE
DPVM=-DWITH_PVM
!ENDIF 

!IF "$(DPVM)" == "-DWITH_PVM"
# compiler flags: -MT should be removed for pvm 
USE_MT=
PVM=libs/pvm.lib 
# SCIDIR1 is set to . in Makefile.mak for compilation 
# and to scilab full path when used after compilation 
# for dynamic linking

!IF "$(DPVM)"==""
PVM_ROOT=$(SCIDIR1)\pvm3
!ELSE
#modify this path to compile scilab with PVM
PVM_ROOT=D:\scilab\pvm3
!ENDIF

PVM_ARCH=WIN32
PVMLIB="$(PVM_ROOT)\lib\WIN32\libpvm3.lib" "$(PVM_ROOT)\lib\WIN32\libgpvm3.lib" 
PVM_CINCLUDE="."
PVM_INCLUDES=-I"$(PVM_ROOT)\include" -I"$(PVM_ROOT)\src"
PVM_CCOMPILER=VISUALC++
!ENDIF 

#
# YES if we compile the PVM given with Scilab else NO
# If you use DLPVM=YES you will have to edit and customize
# pvm3/conf/WIN32.def 
!IF "$(DPVM)"==""
DLPVM=NO
!ELSE
DLPVM=YES
!ENDIF


#--------------------------
# to generate blas symbols compatible with 
# intel blas library 
#--------------------------
#DMKL=-DMKL
#---------------------
# C compiler
# typically, for compiling use: CFLAGS = $(CC_OPTIONS)
# and for linking: $(CC) -o $(PROGRAM) $(OBJS) $(CC_LDFLAGS)
#---------------------
CC=cl
LINKER=link

# standard option for the linker 
LINKER_FLAGS=/NOLOGO /machine:ix86 /RELEASE

# debug for the linker 
#LINKER_FLAGS=/NOLOGO /machine:ix86 /DEBUG

# include options 
INCLUDES=-I"$(SCIDIR)/routines/f2c" $(TCL_INCLUDES) 

CC_COMMON=-D__MSC__ -DWIN32 -c -DSTRICT -D__MAKEFILEVC__ -nologo $(INCLUDES) $(DTK) $(DPVM) $(DMKL) $(USE_MT)

# standard option for C compiler
CC_OPTIONS = $(CC_COMMON) -Z7 -W3 -O2 -G5 -Gd

# debug option for C compiler
#CC_OPTIONS = $(CC_COMMON) -Zi -W3 -Od -GB -Gd

CC_LDFLAGS = 
#---------------------
# Fortran Compiler 
# default usage is to use f2c 
#---------------------
USE_F2C=YES
# if USE_F2C is set to NO we will use the following Fortran compiler (i.e Visual Fortran)
!IF "$(USE_F2C)" == "NO"
FC=df 
FC_OPTIONS=/debug /nologo /assume:underscore /compile_only /iface:(cref,nomixed_str_len_arg) /names:lowercase 
LINKER_FLAGS=$(LINKER_FLAGS) /force:multiple
!ENDIF
#--------------------
# resource compiler 
#--------------------
RC=rc
RCVARS=-r -DWIN32
#--------------------
# Libraries 
#--------------------
GUIFLAGS=-SUBSYSTEM:console
GUI=comctl32.lib wsock32.lib shell32.lib winspool.lib user32.lib gdi32.lib comdlg32.lib kernel32.lib advapi32.lib 

!IF "$(USE_MT)" == "-MT"
GUILIBS=-NODEFAULTLIB:libc.lib -NODEFAULTLIB:msvcrt.lib $(GUI) libcmt.lib oldnames.lib
!ELSEIF "$(USE_MT)" == "-MD"
GUILIBS=-NODEFAULTLIB:libc.lib -NODEFAULTLIB:libcmt.lib $(GUI)  msvcrt.lib
!ELSE 
GUILIBS=-NODEFAULTLIB:libcmt.lib $(GUI) libc.lib msvcrt.lib
!ENDIF

# XLIBS is used for linking Scilab
XLIBS=$(TKLIBS) $(PVMLIB) $(GUILIBS)
# XLIBSBIN is used by the binary version of Scilab for linking examples
XLIBSBIN=$(TKLIBSBIN) $(PVMLIB) $(GUILIBS)
# if you use ATLAS library (dll)
#XLIBSBIN=$(TKLIBSBIN) $(PVMLIB) $(GUILIBS) "$(SCIDIR1)\bin\atlas.lib" "$(SCIDIR1)\bin\libf2c.lib" "$(SCIDIR1)\bin\lapack.lib" "$(SCIDIR1)\bin\arpack.lib"

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
	@"$(SCIDIR1)\bin\f2c.exe" $(FFLAGS) $*.f 
	@$(CC) $(CFLAGS) $*.c 
	-del $*.c 
!ELSE 
.f.obj	:
	@echo -----------Compile file $*.f  (using $(FC)) -------------
	@$(FC) $(FFLAGS) $<
!ENDIF 

#--------------------
# RM only exists if gcwin32 is installed 
#----------------------------------

RM = del

#--------------------
# clean 
#----------------------------------

clean::
	-del *.bak 
  -del *.obj
distclean::  
  -del *.bak 
  -del *.obj
