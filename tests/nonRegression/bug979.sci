// <-- Non-regression test for bug 979 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=979
//
// <-- Short Description -->
//    Matlab comments like %blabla are translated to
//    part(comment,"blabla") [!?]


// Non-regression test file for bug 979
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["comment=1;";"%This line is a comment line"]

MFILE=TMPDIR+"/bug979.m"
SCIFILE=TMPDIR+"/bug979.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"comment = 1;";
		"//This line is a comment line"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,979);
else
	affich_result(%T,979);
end

clear
