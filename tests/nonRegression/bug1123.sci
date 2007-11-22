// <-- Non-regression test for bug 1123 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1123
//
// <-- Short Description -->
//    I think this bug is related to other ones but the matlab
//    construction :
//
//    if(mod(N,2))N=N-1;t=t(1:N);xs=xs(1:N);xp=xp(1:N); x=x(1:N);end
//
//    leads to a error 43 the m2sci script


// Non-regression test file for bug 1123
// Copyright INRIA
// Scilab Project - F. Belahcene

mode(-1);
clear;

MFILECONTENTS="if(true)t=1;end"

MFILE=TMPDIR+"/bug1123.m"
SCIFILE=TMPDIR+"/bug1123.sci"

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
"if %t then t = 1;end;"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,1123);

clear
