// Non-regression test file for bug 1468
// Copyright INRIA
// Scilab Project - F. Belahcene


mode(-1);
clear;

MFILECONTENTS=["beep";
"beep on";
"beep off";
"s = beep()"]

MFILE=TMPDIR+"/bug1468.m"
SCIFILE=TMPDIR+"/bug1468.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR,%f,%t);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
"// Display mode";
"mode(0);"
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"beep";
"beep(""on"")";
"beep(""off"")";
"s = beep()"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
    affich_result(%F,924);
else
    affich_result(%T,924);
end

clear