// Non-regression test file for bug 1123
// Copyright INRIA
// Scilab Project - F. Belahcene

mode(-1);
clear;

MFILECONTENTS="if(true)t=1;end"

MFILE=TMPDIR+"/bug1123.m"
SCIFILE=TMPDIR+"/bug1123.sci"

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
"if %t then t = 1;end;"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,1123);

clear
