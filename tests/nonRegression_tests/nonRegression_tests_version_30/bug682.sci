// Non-regression test file for bug 682
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["b=1;";"if b==0 | b(2)>0";"  a = 1;";"  else a = 0;";"end"]

MFILE=TMPDIR+"/bug682.m"
SCIFILE=TMPDIR+"/bug682.sci"

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
"b = 1;";
"if bool2s(b==0)|bool2s(b(2)>0) then";
"  a = 1;";
"else a = 0;";
"end"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,682);

clear
