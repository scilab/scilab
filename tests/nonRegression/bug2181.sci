// <-- Non-regression test for bug 2181 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2181
//
// <-- Short Description -->
//    getrelativefilename crashes when the two arguments it receives refer to 
//    different drives.
//
//    Francois

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 30 octobre 2006

if MSDOS then
	
	test1 = getrelativefilename("D:\","C:\Program Files\scilab\readme.txt");
	test2 = getrelativefilename("C:\","C:\Program Files\scilab\readme.txt");
	test3 = getrelativefilename("C:\Documents and Settings","C:\Program Files\scilab\readme.txt");
	test4 = getrelativefilename("C:\PROGRAM FILES\toto","c:\program files\scilab\readme.txt");
	
	res1 = ( test1 == "C:\Program Files\scilab\readme.txt" );
	res2 = ( test2 == "C:\Program Files\scilab\readme.txt" );
	res3 = ( test3 == "..\Program Files\scilab\readme.txt" );
	res4 = ( test4 == "..\scilab\readme.txt" );
	
	if (res1 & res2 & res3) then 
		affich_result(%T,2181);
	else
		affich_result(%F,2181);
	end
	
else
	affich_result(%T,2181);
end
