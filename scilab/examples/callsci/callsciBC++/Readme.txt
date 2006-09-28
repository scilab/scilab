How to make calls to Scilab interpreter from Borland C++ 6

Import Scilab library (LibScilab.dll) to Borland

on a Command line

impdef Libscilab.def Libscilab.dll
implib Libscilab.lib Libscilab.def

Add Libscilab.lib in your Borland C++ project in "import library"


(On Google see 'Borland import library')

That's all 

Allan CORNET
September 2006
Scilab Team
