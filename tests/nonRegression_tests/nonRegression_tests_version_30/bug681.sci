// Copyright INRIA
// Scilab Project - V. Couvert
// Non-regression test file for bug 681

mode(-1);
clear all;

MFILECONTENTS=['error(''This is an error message'')'];

MFILE=TMPDIR+"/bug681.m"
SCIFILE=TMPDIR+"/bug681.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(MFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

SCIFILECONTENTSREF=['error(''This is an error message'')'];

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,681);

clear all

