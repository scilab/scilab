
A small example for the link Scilab function 

tdll.sci : is a Scilab script which will run the make utility
and link the function defined in tdll.c 

Makefile and Makefile.DLLs : are used to produce 
a relocatable dll which can be linked into Scilab 

PbS
	[1]  : If you use mingwin32 edit Makefile.DLLs to change 
	the line 
	@echo "-L/softs/GCWIN32/win32/lib" >> libs_path
	if the path is not correct for you 

	[2] : the following line :
		LDFLAGS-libtdll = --image-base=0x11000000 
	      in Makefile should not be present 
	      However it seams necessary at least on Windows95
	      to produce a "linkable" dll for scilab. 
	      Maybe you'll have to change the image-base value 
		on your machine.
