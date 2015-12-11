// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1636 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1636
//
// <-- Short Description -->
//    When processing an m-file with multiple continuation lines in the
//    initial function statement, i.e the first line, I received an index
//    error in  line 181 of m2sci_syntax. On investigation, I found the index
//    k was zero and traced this to lines 141 and 171 which set k=k-1 after
//    appending line k to line k-1. I have produced a modified
//    m2sci_syntax.sci file with the line if k<> 1 then k=k-1; end instead of
//    k=k-1 for lines 141 and 181 which seems to work OK. No errors and
//    produces an sci file

MFILECONTENTS=["function A = bug1636(x,y,...";
"z,t,...";
"u,v,w,...";
"z)";
"a=x+y+z+t+u+v+w+z;"];

MFILE=TMPDIR+"/bug1636.m";
SCIFILE=TMPDIR+"/bug1636.sci";

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR,%f,%t);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [A] = bug1636(x,y,z,t,u,v,w,z)";
"";
"// Output variables initialisation (not found in input variables)"
"A=[];";
"";
"// Display mode";
"mode(0);"
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"a = mtlb_a(mtlb_a(mtlb_a(mtlb_a(mtlb_a(mtlb_a(mtlb_a(x,y),z),t),u),v),w),z);"
"endfunction"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
