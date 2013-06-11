// Non-regression test file for bug 1124
// Copyright INRIA
// Scilab Project - F. Belahcene

mode(-1);
clear;

MFILECONTENTS=[
"load([rep,''alpha'']);";
"load([rep,''beta'']);";
"mu=beta*alpha;";
]

MFILE=TMPDIR+"/bug1124.m"
SCIFILE=TMPDIR+"/bug1124.sci"

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
"// ! L.1: mtlb(rep) can be replaced by rep() or rep whether rep is an M-file or not";
"loadmatfile(mtlb(rep)+""alpha"");";
"// ! L.2: mtlb(rep) can be replaced by rep() or rep whether rep is an M-file or not";
"loadmatfile(mtlb(rep)+""beta"");";
"mu = mtlb_double(beta)*mtlb_double(alpha);";
]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
    correct=%F
end

affich_result(correct,1124);

clear


