// Non-regression test file for bug 943
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

MFILECONTENTS=["a=10;";
"if     (a==1), J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ;";
"elseif (a==2), J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ;";
"elseif (a==3), J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ;";
"else, J = [1 ; 2 ; 3] ; G = [4 ; 5 ; 6] ; end,"]

MFILE=TMPDIR+"/bug943.m"
SCIFILE=TMPDIR+"/bug943.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(SCIFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

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
"else J = [1;2;3]; G = [4;5;6];end"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,943);

clear all
