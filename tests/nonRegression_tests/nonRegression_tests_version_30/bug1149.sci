// Non-regression test file for bug 1149
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Date : 3 mai 2005

mode(-1);
clear;

MFILECONTENTS="a=[,1,2,3,]"

MFILE=TMPDIR+"/bug1149.m"
SCIFILE=TMPDIR+"/bug1149.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
"// Display mode";
"mode(0);";
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"a = [1,2,3]"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,1149);

clear
