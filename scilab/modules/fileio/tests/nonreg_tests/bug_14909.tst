// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- WINDOWS ONLY -->
//
// <-- Non-regression test for bug 14909 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14909
//
// <-- Short Description -->
// getlongpathname and getshortpathname must return value with "\" instead of "/"

p = getlongpathname(SCI);
assert_checkequal(grep(p, "/"), []);

p = getshortpathname(WSCI);
assert_checkequal(grep(p, "/"), []);

