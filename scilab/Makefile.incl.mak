#----------------------------------------------
# version for Visual C++
#----------------------------------------------

all::
MAKE=nmake /f Makefile.mak
#----------------------------------------------
# C compiler
# typically, for compiling use: CFLAGS = $(CC_OPTIONS)
# and for linking: $(CC) -o $(PROGRAM) $(OBJS) $(CC_LDFLAGS)
#----------------------------------------------
CC=cl
LINKER=link

# standard option for the linker
!IF "$(WIN64)" == "OK"
LINKER_FLAGS=/NOLOGO /machine:X64 /RELEASE
!ELSE
LINKER_FLAGS=/NOLOGO /machine:ix86 /RELEASE
!ENDIF

# debug for the linker
!IF "$(WIN64)" == "OK" 
#LINKER_FLAGS=/NOLOGO /machine:X64 /DEBUG
!ELSE
#LINKER_FLAGS=/NOLOGO /machine:ix86 /DEBUG
!ENDIF


# include options 
INCLUDES=-I"$(SCIDIR)\libs\MALLOC\includes" -I"$(SCIDIR)\modules\core\includes" -I"$(SCIDIR)\modules\output_stream\includes" -I"$(SCIDIR)/libs/f2c" -I"$(SCIDIR)/modules/mexlib/includes" 

!IF "$(WIN64)" == "OK" 
CC_COMMON=-D__MSC__ -D_WIN64 -c -DSTRICT -D_CRT_SECURE_NO_DEPRECATE -D__MAKEFILEVC__ -nologo $(INCLUDES) $(DTK) $(USE_MT)
!ELSE
CC_COMMON=-D__MSC__ -DWIN32 -c -DSTRICT -D_CRT_SECURE_NO_DEPRECATE -D__MAKEFILEVC__ -nologo $(INCLUDES) $(DTK) $(USE_MT)
!ENDIF

# standard option for C compiler for VC 2008
CC_OPTIONS = $(CC_COMMON) -Z7 -W3 -O2 -Gd

# debug option for C compiler  for VC 2008
#CC_OPTIONS = $(CC_COMMON) -Zi -W3 -Od -Gd

CC_LDFLAGS = 
#----------------------------------------------
# Fortran Compiler 
# default usage is to use f2c 
#----------------------------------------------
USE_F2C=YES
# if USE_F2C is set to NO we will use the following Fortran compiler (i.e Intel Fortran 9.1)
!IF "$(USE_F2C)" == "NO"
FC=ifort 
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
# XLIBSBIN is used by the binary version of Scilab for linking examples
XLIBSBIN= "$(SCIDIR1)\bin\MALLOC.lib" "$(SCIDIR1)\bin\blasplus.lib" "$(SCIDIR1)\bin\libf2c.lib" "$(SCIDIR1)\bin\lapack.lib" "$(SCIDIR1)\bin\scicos.lib" "$(SCIDIR1)\bin\intersci.lib" "$(SCIDIR1)\bin\scioutput_stream.lib" "$(SCIDIR1)\bin\dynamic_link.lib"
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
	@"$(SCIDIR1)\bin\f2c.exe" -I"$(SCIDIR1)\modules\core\includes" $(FFLAGS) $*.f 2>NUL
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
