// Non-regression test file for bug 699
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["M(1,1) = -sin(0.1);";"M(1,1) = sin(0.1);"]

MFILE=TMPDIR+"/bug699.m"
SCIFILE=TMPDIR+"/bug699.sci"

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
"M = [];M(1,1) = -sin(0.1);";
"M(1,1) = sin(0.1);"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,699);

clear
