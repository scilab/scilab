// <-- Non-regression test for bug 2656 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2656
//
// <-- Short Description -->
//    M2sci doesn't convert "carriage return" inside a loop.

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 15 janvier 2008

MFILECONTENTS=["a = 1;"
                "while ( a == 1)";
                "  a = 2;";
                "end";
                "a = 3"];

MFILE=TMPDIR+"/bug2656.m"
SCIFILE=TMPDIR+"/bug2656.sci"
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
		"a = 1;";
		"while a==1";
		"  a = 2;";
		"end;";
		"a = 3"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
