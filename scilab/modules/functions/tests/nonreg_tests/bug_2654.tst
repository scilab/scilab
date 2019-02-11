// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2654
//
// <-- Short Description -->
// exec returned a wrong error if line had more than 4096 characters

ierr = exec("SCI/modules/functions/tests/nonreg_tests/bug_2654_test_NOK_1.sce","errcatch");
if ierr <> 0 then pause, end

ierr = exec("SCI/modules/functions/tests/nonreg_tests/bug_2654_test_NOK_2.sce","errcatch");
if ierr <> 0 then pause, end

ierr = exec("SCI/modules/functions/tests/nonreg_tests/bug_2654_test_OK.sce","errcatch");
if ierr <> 0 then pause, end

