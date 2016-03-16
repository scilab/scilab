// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Rishubh Jain
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14111 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14111
//
// <-- Short Description -->
//
// lib() did not produce correct error message on detecting Old binary lib file


assert_checkerror("lib(""SCI/modules/io/tests/."")","lib: SCI/modules/io/tests/. is not a valid lib path.");

assert_checkerror("lib(""SCI/modules/io/tests/nonreg_tests/"")","lib: Old binary lib detected. Please recompile it for Scilab 6.");



