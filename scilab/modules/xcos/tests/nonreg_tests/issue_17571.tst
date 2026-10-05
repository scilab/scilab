// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17571 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/work_items/17571
//
// <-- Short Description -->
// The "Unique name" of a SUPER_f block lost its accented characters and a
// name made of digits became empty.

exec(SCI + "/modules/development_tools/tests/unit_tests/javaHelpers.sce", -1);

run_junit_tests(SCI + "/modules/xcos/tests/java/org/scilab/tests/modules/xcos/nonreg_tests/issue_17571.java");
