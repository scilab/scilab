// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16374
//
// .scg files from Scilab 5 with datatips could not be loaded in Scilab 6
assert_checktrue(execstr("load(""SCI/modules/io/tests/nonreg_tests/bug_16374_552.scg"")", "errcatch")==0);
d = gce().children.datatips;
assert_checkequal(d.display_components, ["xyz" ; "xy"]);
