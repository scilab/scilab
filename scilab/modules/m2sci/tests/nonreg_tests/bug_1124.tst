// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1124 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1124
//
// <-- Short Description -->
//    incapacity to use a function name as an identifier

MFILECONTENTS=[
"load([rep,''alpha'']);";
"load([rep,''beta'']);";
"mu=beta*alpha;";
];

MFILE=TMPDIR+"/bug1124.m";
SCIFILE=TMPDIR+"/bug1124.sci";

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
"// ! L.1: mtlb(rep) can be replaced by rep() or rep whether rep is an M-file or not.";
"loadmatfile(mtlb(rep)+""alpha"");";
"// ! L.2: mtlb(rep) can be replaced by rep() or rep whether rep is an M-file or not.";
"loadmatfile(mtlb(rep)+""beta"");";
"mu = beta*alpha;";
]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
