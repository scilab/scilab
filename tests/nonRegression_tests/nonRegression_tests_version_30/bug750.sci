// Non-regression test file for bug 750
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 22 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["t=[];";"cputime;";"t(10)=cputime;"]

MFILE=TMPDIR+"/bug750.m"
SCIFILE=TMPDIR+"/bug750.sci"

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
		"t = [];";
		"timer;";
		"t(1,10) = timer();"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,750);
else
	affich_result(%T,750);
end

clear
