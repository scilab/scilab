// Non-regression test file for bug 942
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["function [a] = bug942 (x) % bug942";"% help line";"% help line";"a = x;"]

MFILE=TMPDIR+"/bug942.m"
SCIFILE=TMPDIR+"/bug942.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [a] = bug942(x) // bug942";
		"";
		"// Ouput variables initialisation (not found in input variables)";
		"a=[];";
		"";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"// help line";
		"// help line";
		"a = x;";
		"endfunction"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,942);
else
	affich_result(%T,942);
end

clear
