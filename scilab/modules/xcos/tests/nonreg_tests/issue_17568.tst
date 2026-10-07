// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17568 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/work_items/17568
//
// <-- Short Description -->
// A scope name with accented characters was not used as the figure name.
// issue_17568.zcos is the diagram of the report, saved before the scope
// name was stored in ipar.

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/issue_17568.zcos"));
Info = xcos_simulate(scs_m, 4);

f = gcf();
assert_checkequal(f.figure_name, "ééé");
close(f);
