// Non-regression test file for bug 925
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["function bug925(A,B)";"A(4)=B(3)"]

MFILE=TMPDIR+"/bug925.m"
SCIFILE=TMPDIR+"/bug925.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [] = bug925(A,B)";
		"";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"A = mtlb_i(A,4,mtlb_e(B,3));";
		"endfunction"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,925);
else
	affich_result(%T,925);
end

clear
