// Non-regression test file for bug 1636
// Copyright INRIA
// Scilab Project - F. Belahcene


mode(-1);
clear;

MFILECONTENTS=["function A = bug1636(x,y,...";
"z,t,...";
"u,v,w,...";
"z)";
"a=x+y+z+t+u+v+w+z;"]

MFILE=TMPDIR+"/bug1636.m"
SCIFILE=TMPDIR+"/bug1636.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR,%f,%t);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [A] = bug1636(x,y,z,t,u,v,w,z)";
"";
"// Ouput variables initialisation (not found in input variables)"
"A=[];";
"";
"// Display mode";
"mode(0);"
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"";
"";
"";
"a = mtlb_a(mtlb_a(mtlb_a(mtlb_a(mtlb_a(mtlb_a(mtlb_a(x,y),z),t),u),v),w),z);"
"endfunction"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
    affich_result(%F,1636);
else
    affich_result(%T,1636);
end

clear