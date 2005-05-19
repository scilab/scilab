// Non-regression test file for bug 859
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["result=ones(10,10);";
		"m=1;";
		"for h=1:10";
		"  result_bis(m,:) = result(h,:);";
		"  m = m + 1;";
		"end"]

MFILE=TMPDIR+"/bug859.m"
SCIFILE=TMPDIR+"/bug859.sci"

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
		"result = ones(10,10);";
		"m = 1;";
		"for h = 1:10";
		"  result_bis(m,1:length(result(h,:))) = result(h,:);";
		"  m = m+1;";
		"end;"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,859);
else
	affich_result(%T,859);
end

clear
