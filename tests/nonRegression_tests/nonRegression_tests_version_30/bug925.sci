// Non-regression test file for bug 925
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["function bug925(A,B)";"A(4)=B(3)"]

MFILE=TMPDIR+"/bug925.m"
SCIFILE=TMPDIR+"/bug925.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(SCIFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

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

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,925);

clear
