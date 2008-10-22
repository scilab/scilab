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
