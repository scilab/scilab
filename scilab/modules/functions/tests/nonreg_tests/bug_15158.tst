// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15158 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15158
//
// <-- Short Description -->
// ver()(1,2) in a macro made macr2tree() crashing on it.

function test()
   SCI_VER  = ver()(1,2)
endfunction

assert_checkequal(execstr("macr2tree(test)", "errcatch"), 0);
