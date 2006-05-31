// Non-regression test file for bug 1082
// Copyright INRIA
// Scilab Project - F.Belahcene

mode(-1);
clear;

MFILECONTENTS=["structure.field = 2;" 
""
"data = round(structure.field);"]

MFILE=TMPDIR+"/bug1082.m"
SCIFILE=TMPDIR+"/bug1082.sci"

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
"structure.field = 2;";
"";
"data = round(mtlb_double(structure.field));"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end
affich_result(correct,1082);

clear
