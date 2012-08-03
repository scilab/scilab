// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 699 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=699
//
// <-- Short Description -->
//    m2sci crashes for a very simple input file
//
//    The content of the offending file a_test.m is a single 
//    line :
//
//    M(1,1) = -sin(0.1);
//
//    When I try to convert this Matlab file to Scilab using the 
//    m2sci application (menu option in the main Scilab window), 
//    the reported error message displays.

MFILECONTENTS=["M(1,1) = -sin(0.1);";"M(1,1) = sin(0.1);"];

MFILE=TMPDIR+"/bug699.m";
SCIFILE=TMPDIR+"/bug699.sci";

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
		"M(1,1) = -sin(0.1);";
		"M(1,1) = sin(0.1);"];


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
