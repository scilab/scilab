// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- TEST WITH ATOMS -->
//
// <-- Non-regression test for bug 13108 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13108
//
// <-- Short Description -->
// The time between two updates can now be configured thanks to atomsSetConfig("updateTime", time)

atomsSaveConfig();

// Set Atoms update time to the default value
atomsSetConfig("updateTime", "30");

assert_checkequal(strtod(atomsGetConfig("updateTime")), 30);

atomsRestoreConfig(%T);
