// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8052 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8052
//
// <-- Short Description -->
// Input/Output values must not be used for getting port number as they may
// contain arbitrary user inputs.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_8052.zcos");
// open a super block
// check that no exception is thrown

