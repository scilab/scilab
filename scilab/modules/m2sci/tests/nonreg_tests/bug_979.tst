// <-- Non-regression test for bug 979 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=979
//
// <-- Short Description -->
//    Matlab comments like %blabla are translated to
//    part(comment,"blabla") [!?]

// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

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

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
