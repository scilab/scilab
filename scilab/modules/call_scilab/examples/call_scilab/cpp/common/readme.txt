	
	How to make calls to Scilab interpreter 
	from a C++ program 
	
In this directory we provide small examples of C++ calls to the Scilab 
interpreter. 

[1] Unix ( soure are myprog.c and ccmatrix1.cc )
To make it run you have to edit the Makefile 
and change the SCIDIR variable to fit your local Scilab installation 

Then type 

	make all 
	./myprog 

[2] Windows (sources are myprog.c and ccmatrix1.cpp )
	nmake /f Makefile.mak all /a
	
	..\..\..\..\..\..\bin\prog.exe

Update INRIA 2007
