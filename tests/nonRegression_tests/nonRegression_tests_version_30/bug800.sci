// Non-regression test file for bug 800
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

MFILECONTENTS=["a=1/-2;";"b=1/-23;";"c=1/-25;";"d=1/ -c;";"e=1 /  -36;x=1/-2";"f=1 / (-36);"]

MFILE=TMPDIR+"/bug800.m"
SCIFILE=TMPDIR+"/bug800.sci"

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
"a = 1/(-2);";
"b = 1/(-23);";
"c = 1/(-25);";
"d = 1/(-c);";
"e = 1/(-36);x = 1/(-2)";
"f = 1/(-36);"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,800);

clear all
