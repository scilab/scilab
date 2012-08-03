// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 979 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=979
//
// <-- Short Description -->
//    Matlab comments like %blabla are translated to
//    part(comment,"blabla") [!?]

MFILECONTENTS=["comment=1;";"%This line is a comment line"];

MFILE=TMPDIR+"/bug979.m";
SCIFILE=TMPDIR+"/bug979.sci";

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
		"comment = 1;";
		"//This line is a comment line"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
