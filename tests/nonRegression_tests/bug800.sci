// Non-regression test file for bug 800
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["a=1/-2;";"b=1/-23;";"c=1/-25;";"d=1/ -c;";"e=1 /  -36;x=1/-2";"f=1 / (-36);"]

MFILE=TMPDIR+"/bug800.m"
SCIFILE=TMPDIR+"/bug800.sci"

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
		"a = 1/(-2);";
		"b = 1/(-23);";
		"c = 1/(-25);";
		"d = 1/(-c);";
		"e = 1/(-36);x = 1/(-2)";
		"f = 1/(-36);"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,800);
else
	affich_result(%T,800);
end

clear
