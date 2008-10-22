// Non-regression test file for bug 945
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["x(1) = 0.;";"x(2) = 0.;";"y = zeros (2,1);";"y(1) = 0.;";"y(2) = 0.;"]

MFILE=TMPDIR+"/bug945.m"
SCIFILE=TMPDIR+"/bug945.sci"

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
		"x(1,1) = 0;";
		"x(1,2) = 0;";
		"y = zeros(2,1);";
		"y(1) = 0;";
		"y = mtlb_i(y,2,0);"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,945);
else
	affich_result(%T,945);
end

clear
