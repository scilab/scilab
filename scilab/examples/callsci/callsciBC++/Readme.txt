How to make calls to Scilab interpreter from Borland C++ 6

	
In this directory we provide a small project for Borland C++ 6
which use the Scilab interpreter to perform matrix computation 

This example was only tested under Windows XP and Borland C++ 6 (Trial)

Important Note: This demo will not work with a binary scilab 
	version. You need to have a locally compiled scilab version 
	(I need main.c file in \routines\libf2c\) 


1] build scilab
2] import Scilab library to Borland

on a Commands line

 implib -a C:\scilab-2.7.2\examples\callsci\callsciBC++\LibScilab.lib C:\scilab-2.7.2\bin\LibScilab.dll
 impdef -a C:\scilab-2.7.2\examples\callsci\callsciBC++\LibScilab.def C:\scilab-2.7.2\bin\LibScilab.dll

3] open project scilab.bpr

That's all 

Allan CORNET
September 2003

