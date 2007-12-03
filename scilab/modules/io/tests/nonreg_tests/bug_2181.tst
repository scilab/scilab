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
	
	if test1 <> "C:\Program Files\scilab\readme.txt" then pause,end
	if test2 <> "C:\Program Files\scilab\readme.txt" then pause,end
	if test3 <> "..\Program Files\scilab\readme.txt" then pause,end
	if test4 <> "..\scilab\readme.txt"               then pause,end
	
end
