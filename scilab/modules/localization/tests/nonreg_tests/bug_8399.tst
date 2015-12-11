//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SQcilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 8399 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8399
//
// <-- Short Description -->
// setlanguage didn't work when only fr_FR.UTF-8 was available.

assert_checkequal(setlanguage("fr_FR"), %t);
assert_checkequal(getlanguage(), "fr_FR");
