// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2137 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2137
//
// <-- Short Description -->
//    wrong translation of Matlab function randn
//
//
//    Put this single line in an m-file:
//
//    R = randn
//
//    and try to convert using mfile2sci.
//
//    Output is:
//
//    R = randn(1,1,"normal")
//
//    which is wrong (no such function in Scilab).
//
//    Francois

MFILECONTENTS=[
"R = randn;";
];

MFILE=TMPDIR+"/bug2137.m";
SCIFILE=TMPDIR+"/bug2137.sci";

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
];

correct=%T;
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
