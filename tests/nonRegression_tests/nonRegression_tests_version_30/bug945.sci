// Non-regression test file for bug 945
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["x(1) = 0.;";"x(2) = 0.;";"y = zeros (2,1);";"y(1) = 0.;";"y(2) = 0.;"]

MFILE=TMPDIR+"/bug945.m"
SCIFILE=TMPDIR+"/bug945.sci"

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
"x = [];x(1,1) = 0;";
"x(1,2) = 0;";
"y = zeros(2,1);";
"y(1) = 0;";
"y = mtlb_i(y,2,0);"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,945);

clear
