// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->

// <-- Short Description -->
// Runs the JUnit tests of Xcos (tests/java) in a separate JVM.
// Needs the JVM, so it is skipped in -nwni mode.

exec(SCI + "/modules/development_tools/tests/unit_tests/javaHelpers.sce", -1);

run_junit_tests(SCI + "/modules/xcos/tests/java");
