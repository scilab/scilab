// Non-regression test file for bug 946
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

MFILECONTENTS=["x=zeros(3,2);";"y=zeros(3,1);";"x=[1 0;0 0;0 0];";"y(1:3)=x(:,1);"]

MFILE=TMPDIR+"/bug946.m"
SCIFILE=TMPDIR+"/bug946.sci"

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
"x = zeros(3,2);";
"y = zeros(3,1);";
"x = [1,0;0,0;0,0];";
"y(1:3) = x(:,1);"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,946);

clear all
