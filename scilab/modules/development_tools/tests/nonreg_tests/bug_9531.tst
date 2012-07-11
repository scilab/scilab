// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 9531 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9531
//
// <-- Short Description -->
// assert_checkequal fails with mlist


M_ref=mlist(['V','name','value'],['a','b';'c' 'd'],[1 2; 3 4]);
M_ref2=M_ref; 
assert_checkequal(M_ref, M_ref2);
//
M_ref=tlist(['V','name','value'],['a','b';'c' 'd'],[1 2; 3 4]);
M_ref2=M_ref; 
assert_checkequal(M_ref, M_ref2);

