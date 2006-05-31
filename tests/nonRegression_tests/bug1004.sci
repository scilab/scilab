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

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,1004);
else
	affich_result(%T,1004);
end

clear
