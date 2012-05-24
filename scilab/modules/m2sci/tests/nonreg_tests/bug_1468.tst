// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1468 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1468
//
// <-- Short Description -->
//    1) 'macr2tree' function doesn't work correctly with the following script

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


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
