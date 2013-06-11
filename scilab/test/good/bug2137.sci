// Non-regression test file for bug 2137
// Copyright INRIA
// Scilab Project - F. Belahcene

mode(-1);
clear;

MFILECONTENTS=[
"R = randn;";
]

MFILE=TMPDIR+"/bug2137.m"
SCIFILE=TMPDIR+"/bug2137.sci"

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
"R = rand(1,1,""normal"");";
]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
    correct=%F
end

affich_result(correct,2137);

clear


