// Non-regression test file for bug 1971
// Copyright INRIA
// Scilab Project - F. Belahcene

mode(-1);
clear;

MFILECONTENTS=[
"fprintf(''qsdfsdcvwqghcv'')";
]

MFILE=TMPDIR+"/bug1971.m"
SCIFILE=TMPDIR+"/bug1971.sci"

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
"// L.1: No simple equivalent, so mtlb_fprintf() is called";
"mtlb_fprintf(""qsdfsdcvwqghcv"")";
]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
    correct=%F
end
instr="mtlb_fprintf(""qsdfsdcvwqghcv"")";
ierr=execstr(instr,"errcatch")
if ierr<>0 then correct=%F, end
affich_result(correct,1971);

clear


