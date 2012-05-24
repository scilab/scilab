// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 925 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=925
//
// <-- Short Description -->
//    Bad insertion conversion
//
//    Following Matlab code:
//    function test(A,B)
//    A(3)=B(4);
//
//    is converted to Scilab code :
//    function [] = test(A,B)
//
//    // Display warning for floating point exception
//    ieee(1)
//
//    A = mtlb_i(A,1,mtlb_e(B,3));
//    endfunction
//
//    what is obviously wrong !
//    4 has become 1 ....

MFILECONTENTS=["function bug925(A,B)";"A(4)=B(3)"];

MFILE=TMPDIR+"/bug925.m";
SCIFILE=TMPDIR+"/bug925.sci";

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [] = bug925(A,B)";
		"";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"A = mtlb_i(A,4,mtlb_e(B,3));";
		"endfunction"];


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
