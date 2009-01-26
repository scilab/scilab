#==================================================
# Visual C++/Intel Fortran 10.x 
# Allan CORNET - 2008
#==================================================
all::
MAKE=nmake /f Makefile.mak
#==================================================
# C compiler
# typically, for compiling use: CFLAGS = $(CC_OPTIONS)
# and for linking: $(CC) -o $(PROGRAM) $(OBJS) $(CC_LDFLAGS)
#==================================================
CC=cl
LINKER=link

!IF "$(WIN64)" == "OK" 
MACHINE=/machine:X64
DWIN=-D_WIN64
!ELSE
MACHINE=/machine:ix86
DWIN=-DWIN32
!ENDIF

!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
LINKER_OPTIMISATION_MODE=/DEBUG -PDB:$(LIBRARY).pdb
CC__OPTIMISATION_MODE=-Zi -Od 
!ELSE
LINKER_OPTIMISATION_MODE=/RELEASE
CC__OPTIMISATION_MODE=-Z7 -O2
!ENDIF

CC_COMMON=-D__MSC__ $(DWIN) -c -DSTRICT -D_CRT_SECURE_NO_DEPRECATE -D__MAKEFILEVC__ -nologo $(INCLUDES) $(DTK) $(USE_MT)
LINKER_FLAGS=/NOLOGO $(MACHINE) $(LINKER_OPTIMISATION_MODE)
CC_OPTIONS = $(CC_COMMON) -W3 -Gd $(CC__OPTIMISATION_MODE)

# include options 
INCLUDES=-I"$(SCIDIR)\libs\MALLOC\includes" \
-I"$(SCIDIR)\modules\core\includes" \
-I"$(SCIDIR)\modules\output_stream\includes" \
-I"$(SCIDIR)\modules\jvm\includes" \
-I"$(SCIDIR)\modules\localization\includes" \
-I"$(SCIDIR)\libs\intl" \
-I"$(SCIDIR)/libs/f2c" \
-I"$(SCIDIR)/modules/mexlib/includes" \
-I"$(SCIDIR)/modules/localization/includes" \
-I"$(SCIDIR)/modules/jvm/includes" \
-I"$(SCIDIR)/libs/intl"


CC_LDFLAGS = 
#==================================================
# Fortran Compiler 
# default usage is to use f2c 
#==================================================
# detect intel fortran compiler 10.x family
!IF "$(IFORT_COMPILER10)" == ""
USE_F2C=YES
!ELSE
USE_F2C=NO
!ENDIF

# if USE_F2C is set to NO we will use the following Fortran compiler (i.e Intel Fortran 10.x)
!IF "$(USE_F2C)" == "NO"
FC=ifort 
FC_OPTIONS=/debug /nologo /assume:underscore /compile_only /iface:cref /names:lowercase 
LINKER_FLAGS=$(LINKER_FLAGS) /force:multiple
!ENDIF
#==================================================
# resource compiler 
#==================================================
RC=rc
RCVARS=-r -DWIN32
#==================================================
# Libraries 
#==================================================
GUIFLAGS=-SUBSYSTEM:console
GUI=comctl32.lib wsock32.lib shell32.lib winspool.lib user32.lib gdi32.lib comdlg32.lib kernel32.lib advapi32.lib 
GUILIBS=-NODEFAULTLIB:libc.lib -NODEFAULTLIB:msvcrt.lib $(GUI) libcmt.lib oldnames.lib

# XLIBSBIN is used by the binary version of Scilab for linking examples
XLIBSBIN="$(SCIDIR1)\bin\MALLOC.lib" "$(SCIDIR1)\bin\blasplus.lib" \
"$(SCIDIR1)\bin\libf2c.lib" "$(SCIDIR1)\bin\lapack.lib" \
"$(SCIDIR1)\bin\scicos.lib" "$(SCIDIR1)\bin\intersci.lib" \
"$(SCIDIR1)\bin\scioutput_stream.lib" "$(SCIDIR1)\bin\dynamic_link.lib" \
"$(SCIDIR1)\bin\integer.lib" "$(SCIDIR1)\bin\optimization_f.lib" \
"$(SCIDIR1)\bin\libjvm.lib" "$(SCIDIR1)\bin\scilocalization.lib" \
"$(SCIDIR1)\bin\libintl.lib" "$(SCIDIR1)\bin\linpack_f.lib"
#==================================================
.c.obj	:
	@echo ------------- Compile file $< --------------
	$(CC) $(CFLAGS) $< 

.cxx.obj	:
	@echo ------------- Compile file $< --------------
	@$(CC) $(CFLAGS) /EHsc $*.cxx 

.cpp.obj	:
	@echo ------------- Compile file $< --------------
	@$(CC) $(CFLAGS) /EHsc $*.cpp

# default rule for Fortran 77 & 90 Compilation 

!IF "$(USE_F2C)" == "YES"

.f.obj	:
	@echo ----------- Compile file $*.f (using f2c) -------------
!IF "$(F2C_IMPORT_COMMON)" == "YES"	
	@"$(SCIDIR1)/bin/f2c.exe" -E -I"$(SCIDIR1)/modules/core/includes" $(FFLAGS) $*.f 2>NUL
!ELSE	
	@"$(SCIDIR1)/bin/f2c.exe" -I"$(SCIDIR1)/modules/core/includes" $(FFLAGS) $*.f 2>NUL
!ENDIF
	@$(CC) $(CFLAGS) $*.c 
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"

!ELSE
	-del $*.c 
!ENDIF
	
!ELSE 

.f.obj	:
	@echo -----------Compile file $*.f  (using $(FC)) -------------
	@$(FC) $(FFLAGS) $<
	
!ENDIF

!IF "$(USE_F2C)" == "YES"

.f90.obj	:
	@echo F2C cannot build .f90 file
	
!ELSE 

.f90.obj	:
	@echo -----------Compile file $*.f90  (using $(FC)) -------------
	@$(FC) $(FFLAGS) $<
	
!ENDIF

#==================================================
# clean 
RM = del
#==================================================
clean::
	-del *.bak 
  -del *.obj
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
  -del *.pdb
!ENDIF
#==================================================
distclean::  
  -del *.bak 
  -del *.obj
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
  -del *.pdb
!ENDIF
#==================================================

