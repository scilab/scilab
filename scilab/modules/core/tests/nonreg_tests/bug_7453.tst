// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NOT FIXED --> 6.0.0
//
// <-- Non-regression test for bug 7453 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7453
//
// <-- Short Description -->
// after hashtable rewrite, it was not possible anymore to call newfun()
// after removing the funptr with clearfun()
//
//

id = funptr("sin");
assert_checktrue(id <> 0);
assert_checktrue(clearfun("sin"));
newfun("sin", id);
assert_checktrue(sin(0) == 0);
