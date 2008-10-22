// Non-regression test file for bug 924
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["function bug924";
		"a=''milou'';";
		"a=''milou'',";
		"a=''milou''";
		"a;";
		"a,";
		"a";
		"[c,d]=svd(1);";
		"[c,d]=svd(1),";
		"[c,d]=svd(1)";
		"b=a;";
		"b=a,";
		"b=a"]

MFILE=TMPDIR+"/bug924.m"
SCIFILE=TMPDIR+"/bug924.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [] = bug924()";
		"";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"a = ""milou"";";
		"a = ""milou"",";
		"a = ""milou""";
		"a;";
		"a,";
		"a";
		"[c,d] = svd(1);";
		"[c,d] = svd(1),";
		"[c,d] = svd(1)";
		"b = a;";
		"b = a,";
		"b = a";
		"endfunction"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,924);
else
	affich_result(%T,924);
end

clear
