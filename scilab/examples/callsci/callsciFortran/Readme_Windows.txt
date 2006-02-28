    An example of calling scilab as a subroutine of a user's main program


Useful files
============

    * callsci.f: Fortran Example of a usermain program. This main program send matrices 
                (call matz) and commands (call scirun) to scilab and 
                 gets the results back (call matz).

Usage
=====
	from this directory :
	
	nmake /f makefile.mak all /a
	set SCI=<your current scilab path ex: d:/scilab> 
	set TMPDIR=%TEMP%
	set HOME=%SCI%
	set TCL_LIBRARY=%SCI%\tcl\tcl8.4
	set TK_LIBRARY=%SCI%\tcl\tk8.4
	
	..\..\..\bin\prog.exe

INRIA 2006 A.C
