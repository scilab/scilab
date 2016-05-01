//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 13981 -->
//
// <-- Short Description -->
// lib(path) corrupts the first on error
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13981
//

path = "SCI/test";
refMsg = msprintf(_("%s: %s is not a valid lib path.\n"), "lib", path);
assert_checkerror("lib(path)", refMsg);
