// Non-regression test file for bug 943
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["a=10;";
		"if     (a==1), J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ;";
		"elseif (a==2), J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ;";
		"elseif (a==3), J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ;";
		"else, J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ; end,"]

MFILE=TMPDIR+"/bug943.m"
SCIFILE=TMPDIR+"/bug943.sci"

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
		"a = 10;";
		"if a==1 then J = [1;2;3]; G = [4;5;6];";
		"elseif a==2 then J = [1;2;3]; G = [4;5;6];";
		"elseif a==3 then J = [1;2;3]; G = [4;5;6];";
		"else J = [1;2;3]; G = [4;5;6];end;"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,943);
else
	affich_result(%T,943);
end

clear
