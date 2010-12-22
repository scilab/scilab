// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 4399 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4399
//
// <-- Short Description -->
// It was not possible to distinguish lines from inner functions and from normal functions

tmpfile = TMPDIR + "/bug_4399.sce";
mputl(["a" "function foo()" "b" "c" "function bar()" "a" "b" "c" "endfunction" "d" "e" "endfunction"], tmpfile);
scinotes(tmpfile);

// Change the line numbering to be in whereami mode
// You must see two differents colors to distinguish lines from foo() and from bar()