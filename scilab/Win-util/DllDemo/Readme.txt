
an example of a relocatable dll with cygwin32 
type make 
then ./test1.exe  and ./test2.exe 

If you use mingwin32 edit Makefile.DLLs to change 
the line 

@echo "-L/softs/GCWIN32/win32/lib" >> libs_path

if the path isn't correct for you 


