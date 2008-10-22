// Non-regression test file for bug 682
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 22 Mar 2005

mode(-1);
clear;

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

clear
