 // Non-regression test file for bug 1149
// Copyright INRIA
// Scilab Project - F.Belahcene

mode(-1);
clear;

MFILECONTENTS="a=[,1,2,3,];"

MFILE=TMPDIR+"/bug1149.m"
SCIFILE=TMPDIR+"/bug1149.sci"

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
"a = [1,2,3];"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end
affich_result(correct,1149);

clear
