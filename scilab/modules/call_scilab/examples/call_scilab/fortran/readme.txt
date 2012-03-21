    An example of calling scilab as a subroutine of a user's main program
    
    Requirements Intel Fortran 2011 and Visual Studio 2010 IDE


Useful files
============

    * callsci.f: Fortran Example of a usermain program. This main program send matrices 
                (call matz) and commands (call scirun) to scilab and 
                 gets the results back (call matz).

Usage
=====
	from this directory :
	
	set SCI=<your current scilab path ex: d:/scilab> 
	set TMPDIR=%TEMP%
	set HOME=%SCI%
	set TCL_LIBRARY=%SCI%\modules\tclsci\tcl\tcl8.5
	set TK_LIBRARY=%SCI%\modules\tclsci\tcl\tk8.5
	
	../../../../bin/example.exe

INRIA 2006 A.C
DIGITEO 2012 - Allan CORNET
