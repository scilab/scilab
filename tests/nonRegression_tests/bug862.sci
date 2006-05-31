// Non-regression test file for bug 862
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS="a(1)=0;"

MFILE=TMPDIR+"/bug862.m"
SCIFILE=TMPDIR+"/bug862.sci"

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
"a(1,1) = 0;"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,862);

clear
