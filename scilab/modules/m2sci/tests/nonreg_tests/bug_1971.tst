// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1971 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1971
//
// <-- Short Description -->
//    the mtlb_fprintf does not work. It should be re-written to follow the Matlab doc
//    of fprintf

MFILECONTENTS=[
"fprintf(''qsdfsdcvwqghcv'')";
];

MFILE=TMPDIR+"/bug1971.m";
SCIFILE=TMPDIR+"/bug1971.sci";

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
"// L.1: No simple equivalent, so mtlb_fprintf() is called.";
"mtlb_fprintf(""qsdfsdcvwqghcv"")";
];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end

instr = 'mtlb_fprintf(""qsdfsdcvwqghcv"")';
ierr  = execstr(instr,'errcatch');
if ierr<>0  then pause,end
