
SCIDIR=..\..

include ../../Makefile.incl.mak

all :: info

info:
	@echo Type "nmake /f Makefile.mak tests" in tests directory
	@echo   to test the  distribution
	@echo  SEE THE README FILE (IMPORTANT)
	
tests:
	$(SCIDIR)\bin\scilex -nw -f maketests.sce
	
clean ::
	@del *.dia
	@del Makelib
	@del fun1.*
	@del *.c
	@del *.o
	@del *.obj
	@del libext1c.*
	@del libfoo*
	@del loader.sce
	@del *.dat
	@del asave
	@del foo
	@del fooc.*
	@del foof.*

distclean:: clean 
