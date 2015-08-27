//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug J -->
//
// <-- OLD BUG -->
//
// crash Scilab

function toto()
    N
endfunction

assert_checkequal(execstr("toto()","errcatch"), 999);
