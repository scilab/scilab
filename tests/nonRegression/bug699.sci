// <-- Non-regression test for bug 699 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=699
//
// <-- Short Description -->
//    m2sci crashes for a very simple input file
//
//    The content of the offending file a_test.m is a single 
//    line :
//
//    M(1,1) = -sin(0.1);
//
//    When I try to convert this Matlab file to Scilab using the 
//    m2sci application (menu option in the main Scilab window), 
//    the reported error message displays.


// Non-regression test file for bug 699
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 22 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["M(1,1) = -sin(0.1);";"M(1,1) = sin(0.1);"]

MFILE=TMPDIR+"/bug699.m"
SCIFILE=TMPDIR+"/bug699.sci"

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
		"M(1,1) = -sin(0.1);";
		"M(1,1) = sin(0.1);"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,699);
else
	affich_result(%T,699);
end

clear
