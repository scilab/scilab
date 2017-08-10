// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13983 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13983
//
// <-- Short Description -->
// who_user returns wrong values ( %e, %i, ...)

vars = who_user(%f);
assert_checkequal(grep(vars, ["%e", "%i", "%inf"]), []);
