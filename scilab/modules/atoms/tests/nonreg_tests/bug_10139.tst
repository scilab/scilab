// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 10139 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10139
//
// <-- Short Description -->
// The atomsTest function did not manage a specific test name.

// Install a module

atomsInstall('apifun')
atomsTest('apifun')
atomsTest('apifun', 'checkreal')
atomsTest('apifun', ['checkreal', 'complete'])
