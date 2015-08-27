// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 13971 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13971
//
// <-- Short Description -->
// add a space between prompt and cursor

//test_run mechanism remove prompt for tst or dia.ref file
//create it by string addition
assert_checkequal(prompt(), "--" + "> ");