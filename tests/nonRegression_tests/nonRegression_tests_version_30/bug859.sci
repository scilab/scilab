// Non-regression test file for bug 859
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

MFILECONTENTS=["result=ones(10,10);";
"m=1;";
"for h=1:10";
"  result_bis(m,:) = result(h,:);";
"  m = m + 1;";
"end"]

MFILE=TMPDIR+"/bug859.m"
SCIFILE=TMPDIR+"/bug859.sci"

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
"result = ones(10,10);";
"m = 1;";
"result_bis = [];";
"for h = 1:10";
"  result_bis(m,1:length(result(h,:))) = result(h,:);";
"  m = m+1;";
"end"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,859);

clear all
