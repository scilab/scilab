How to compile with visual studio C++ .Net 2003 and Intel Fortran 9.1 ?

Prerequirements:

-- Visual Studio C++ .NET 2003
-- Intel Fortran 9.1.3192
-- Java JDK 1.5

-- TCL/TK 8.4(.13)
-- zlib 1.2.3
-- pvm 3.4

Please download prerequirements for Scilab (TCL/TK,pvm,...) on Scilab Website
http://www.scilab.org/... (not yet on website)

or on SVN :

md Prerequirements
cd Prerequirements
svn co --username anonymous --password Scilab svn://frioul.inria.fr/scilab/trunk/Dev-Tools/Prerequirements/Windows

copy all files in your scilab directory

click on scilab.sln (to launch Visual) or devenv scilab.sln /build "Release with Atlas" (via command line, to build scilab)
