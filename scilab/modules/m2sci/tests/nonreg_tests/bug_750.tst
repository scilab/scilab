// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 750 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=750
//
// <-- Short Description -->
//    I try to translate to Scilab the benchmark m-file of matlab.
//    This should give a comparison of scilab and matlab execution
//    times.
//
//    It does not translate. Too bad...

MFILECONTENTS=["t=[];";"cputime;";"t(10)=cputime;"];

MFILE=TMPDIR+"/bug750.m";
SCIFILE=TMPDIR+"/bug750.sci";

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
		"t = [];";
		"timer;";
		"t(1,10) = timer();"];


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
