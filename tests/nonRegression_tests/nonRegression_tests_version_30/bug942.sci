// Non-regression test file for bug 942
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["function [a] = bug942 (x) % bug942";"% help line";"% help line";"a = x;"]

MFILE=TMPDIR+"/bug942.m"
SCIFILE=TMPDIR+"/bug942.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(SCIFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

SCIFILECONTENTSREF=["function [a] = bug942(x) // bug942";
"";
"// Ouput variables initialisation (not found in input variables)";
"a=[];";
"";
"// Display mode";
"mode(0);";
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"// help line";
"// help line";
"a = x;";
"endfunction"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,942);

clear
