// <-- Non-regression test for bug 1004 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1004
//
// <-- Short Description -->
//    disp added during translation of Matlab for loop
//
//    Try to convert the following 3-line file ct2.m:
//
//    function ct2()
//    for i=1:5
//    end
//
//
//    The sci file created is:
//
//    function [] = ct2()
//
//    // Display warning for floating point exception
//    ieee(1)
//
//    for i = 1:5,
//      disp(i,"i  =");
//    end
//    endfunction
// ...


// Non-regression test file for bug 1004
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["for i=1:5";"end"]

MFILE=TMPDIR+"/bug1004.m"
SCIFILE=TMPDIR+"/bug1004.sci"

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
		"for i = 1:5";
		"end;"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
