// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 682 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=682
//
// <-- Short Description -->
//    probleme with the logical or in tests.

MFILECONTENTS=["b=1;";"if b==0 | b(2)>0";"  a = 1;";"  else a = 0;";"end"];

MFILE=TMPDIR+"/bug682.m";
SCIFILE=TMPDIR+"/bug682.sci";

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
		"b = 1;";
		"if b==0 | b(2)>0 then";
		"  a = 1;";
		"else a = 0;";
		"end;"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
