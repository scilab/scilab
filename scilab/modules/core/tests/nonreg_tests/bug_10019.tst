// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 10019 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10019
//
// <-- Short Description -->
// 'exec' crashed on the error 113

ierr = exec('SCI/modules/core/tests/nonreg_tests/bug_10019.sce','errcatch');
assert_checkequal(ierr, 0);
assert_checkequal(lasterror(), []);
