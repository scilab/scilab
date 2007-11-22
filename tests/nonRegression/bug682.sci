// <-- Non-regression test for bug 682 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=682
//
// <-- Short Description -->
//    probleme with the logical or in tests.

// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 22 Mar 2005

MFILECONTENTS=["b=1;";"if b==0 | b(2)>0";"  a = 1;";"  else a = 0;";"end"]

MFILE=TMPDIR+"/bug682.m"
SCIFILE=TMPDIR+"/bug682.sci"

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
		"b = 1;";
		"if bool2s(b==0)|bool2s(b(2)>0) then";
		"  a = 1;";
		"else a = 0;";
		"end;"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,682);
else
	affich_result(%T,682);
end
