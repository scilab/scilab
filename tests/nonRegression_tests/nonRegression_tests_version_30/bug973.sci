// Non-regression test file for bug 973
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["%m2scideclare var|Unknown Unknown|Unknown";"var1 = strrep(var,'' '','','')"]

MFILE=TMPDIR+"/bug973.m"
SCIFILE=TMPDIR+"/bug973.sci"

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
"";
"var1 = mtlb_strrep(var,"" "","","")"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,973);

clear
