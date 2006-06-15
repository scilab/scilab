How to make calls to Scilab interpreter from Borland C++ 6
	
In this directory we provide a small project for Borland C++ 6
which use the Scilab interpreter to perform matrix computation 

This example was only tested under Windows XP and Borland C++ 6 (Trial)

import Scilab library to Borland

on a Commands line

 implib -a C:\scilab-5.0\examples\callsci\callsciBC++\LibScilab.lib C:\scilab-5.0\bin\LibScilab.dll
 impdef -a C:\scilab-5.0\examples\callsci\callsciBC++\LibScilab.def C:\scilab-5.0\bin\LibScilab.dll

3] open project scilab.bpr

That's all 

Allan CORNET
INRIA 2006

