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
DIR_OBJ=Debug
LINKER_OPTIMISATION_MODE=/DEBUG -PDB:"$(DIR_OBJ)\$(LIBRARY).pdb"
CC__OPTIMISATION_MODE=-Zi -Od -MDd
!ELSE
DIR_OBJ=Release
LINKER_OPTIMISATION_MODE=/RELEASE 
CC__OPTIMISATION_MODE=-Z7 -O2 -MD -DNDEBUG
!ENDIF

CC_COMMON=-D__MSC__ -DFORDLL $(DWIN) -c -DSTRICT -D_CRT_SECURE_NO_DEPRECATE -D__MAKEFILEVC__ -nologo $(INCLUDES)
LINKER_FLAGS=/NOLOGO $(MACHINE) /DYNAMICBASE:NO /NXCOMPAT:NO $(LINKER_OPTIMISATION_MODE)
CC_OPTIONS = $(CC_COMMON) -W3 -Gd $(CC__OPTIMISATION_MODE) /Fo"$(DIR_OBJ)/" /Fd"$(DIR_OBJ)/"
#==================================================
CC_LDFLAGS = 
#==================================================
# Fortran Compiler 
# default usage is to use f2c 
#==================================================
USE_F2C=YES
# detect intel fortran compiler
!IF "$(IFORT_COMPILER9)" == ""
!ELSE
USE_F2C=NO
!ENDIF
!IF "$(IFORT_COMPILER10)" == ""
!ELSE
USE_F2C=NO
!ENDIF
!IF "$(IFORT_COMPILER11)" == ""
!ELSE
USE_F2C=NO
!ENDIF
!IF "$(IFORT_COMPILER12)" == ""
!ELSE
USE_F2C=NO
!ENDIF
!IF "$(IFORT_COMPILER13)" == ""
!ELSE
USE_F2C=NO
!ENDIF
!IF "$(IFORT_COMPILER14)" == ""
!ELSE
USE_F2C=NO
!ENDIF

#==================================================
# if USE_F2C is set to NO we will use the following Fortran compiler (i.e Intel Fortran 10.x)
!IF "$(USE_F2C)" == "NO"
FC=ifort 
FC_OPTIONS_COMMON=/nologo /DFORDLL /assume:underscore \
/noaltparam /f77rtl /fpscomp:nolibs /names:lowercase \
/iface:cref /threads /c /Qvc9 \
/Fo"$(DIR_OBJ)/" /Fd"$(DIR_OBJ)/" \
/include:"$(SCIDIR)/modules/core/includes"
#==================================================
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
FC_OPTIONS=$(FC_OPTIONS_COMMON) /Zi /Od /debug /dbglibs
#==================================================
!ELSE
FC_OPTIONS=$(FC_OPTIONS_COMMON)
!ENDIF
#==================================================
LINKER_FLAGS=$(LINKER_FLAGS) /force:multiple
!ENDIF
#==================================================
# resource compiler 
#==================================================
RC=rc
RCVARS=-r -DWIN32
#==================================================
# default rules for Fortran 77 & 90 Compilation 
#==================================================
!IF "$(USE_F2C)" == "YES"
#==================================================
# F2C
#==================================================
.f{$(DIR_OBJ)}.obj	:
	@echo ----------- Compile file $< (using f2c) -------------
!IF "$(F2C_IMPORT_COMMON)" == "YES"	
	@"$(SCIDIR)/bin/f2c.exe" -E -I"$(SCIDIR)/modules/core/includes" $(FFLAGS) $< 2>NUL
!ELSE
	@"$(SCIDIR)/bin/f2c.exe" -I"$(SCIDIR)/modules/core/includes" $(FFLAGS) $< 2>NUL
!ENDIF
	-IF NOT EXIST  $(DIR_OBJ) mkdir $(DIR_OBJ)

	@$(CC) $(CFLAGS) $(<:.f=.c)
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
!ELSE
  -del $(<:.f=.c)
!ENDIF
.f90{$(DIR_OBJ)}.obj	:
	@echo F2C cannot build .f90 file	
!ELSE
#==================================================
# INTEL FORTRAN
#==================================================
.f{$(DIR_OBJ)}.obj	:
	@echo ----------- Compile file $< (using INTEL FORTRAN) -------------
	-IF NOT EXIST  $(DIR_OBJ) mkdir $(DIR_OBJ)
	@$(FC) $(FFLAGS) $<
.f90{$(DIR_OBJ)}.obj	:
	@echo ----------- Compile file $< (using INTEL FORTRAN 90) -------------
	-IF NOT EXIST  $(DIR_OBJ) mkdir $(DIR_OBJ)
	@$(FC) $(FFLAGS) $<
!ENDIF
#==================================================
# default rules for C++
#==================================================
.cxx{$(DIR_OBJ)}.obj	:
	@echo ------------- Compile file $< --------------
	-IF NOT EXIST  $(DIR_OBJ) mkdir $(DIR_OBJ)
	@$(CC) $(CFLAGS) /EHsc $< 
#==================================================
.cpp{$(DIR_OBJ)}.obj	:
	@echo ------------- Compile file $< --------------
	-IF NOT EXIST  $(DIR_OBJ) mkdir $(DIR_OBJ)
	@$(CC) $(CFLAGS) /EHsc $<
#==================================================
# default rules for C
#==================================================
.c{$(DIR_OBJ)}.obj	:
	@echo ------------- Compile file $< --------------
	-IF NOT EXIST  $(DIR_OBJ) mkdir $(DIR_OBJ)
	$(CC) $(CFLAGS) $< 
#==================================================
# clean 
RM = del
#==================================================
clean::
	-del *.bak 
  -del "$(DIR_OBJ)\*.obj"
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
  -del "$(DIR_OBJ)\*.pdb"
!ENDIF
  -rmdir "$(DIR_OBJ)"
#==================================================
distclean::  
  -del *.bak 
  -del "$(DIR_OBJ)\*.obj"
!IF "$(DEBUG_SCILAB_DYNAMIC_LINK)" == "YES"
  -del "$(DIR_OBJ)\*.pdb"
!ENDIF
  -rmdir "$(DIR_OBJ)"
#==================================================

