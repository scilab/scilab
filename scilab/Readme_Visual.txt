----------------
Prerequirements:
----------------

* Visual Studio C++ .NET 2005 Pro
* Intel Fortran 9.1.3192
or 

* Visual Studio C++ .NET 2005 Express
* Microsoft Plateform SDK R2
(http://msdn.microsoft.com/vstudio/express/visualc/usingpsdk/)


* download prerequirements for Scilab (TCL/TK,pvm,...) on Scilab Website
http://www.scilab.org/... (not yet on website)

or on SVN :

md Prerequirements
cd Prerequirements
svn co --username anonymous --password Scilab svn://frioul.inria.fr/scilab/trunk/Dev-Tools/Prerequirements/Windows

copy all files in your scilab directory

-----------------------------------------------------------------------
How to compile with visual studio C++ .Net 2005 and Intel Fortran 9.1 ?
-----------------------------------------------------------------------

click on scilab.sln (to launch Visual) or devenv scilab.sln /build "Release" (via command line, to build scilab)


-----------------------------------------------------------------------
How to compile with visual studio C++ .Net 2005 Express ?
-----------------------------------------------------------------------

this version uses f2c to convert fortran to C

click on scilab_f2c.sln (to launch Visual) or devenv scilab_f2c.sln /build "Release" (via command line, to build scilab)
