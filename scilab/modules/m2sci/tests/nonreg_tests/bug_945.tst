// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 945 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=945
//
// <-- Short Description -->
//    mfile2sci fails in case of non pre-sized vectors (all 
//    assignments are done for the first component).
//
//    Test.m leads to Test.sci (obviously wrong)
//    Test2.m leads to Test2.sci

MFILECONTENTS=["x(1) = 0.;";"x(2) = 0.;";"y = zeros (2,1);";"y(1) = 0.;";"y(2) = 0.;"];

MFILE=TMPDIR+"/bug945.m";
SCIFILE=TMPDIR+"/bug945.sci";

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
		"x(1,1) = 0;";
		"x(1,2) = 0;";
		"y = zeros(2,1);";
		"y(1) = 0;";
		"y = mtlb_i(y,2,0);"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
