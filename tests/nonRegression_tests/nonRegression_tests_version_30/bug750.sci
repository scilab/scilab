// Non-regression test file for bug 750
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

MFILECONTENTS=["t=[];";"cputime;";"t(10)=cputime;"]

MFILE=TMPDIR+"/bug750.m"
SCIFILE=TMPDIR+"/bug750.sci"

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
"t = [];";
"timer;";
"t(1,10) = timer();"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,750);

clear all
