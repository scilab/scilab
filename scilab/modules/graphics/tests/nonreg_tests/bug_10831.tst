// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 10831 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10831
//
// <-- Short Description -->
// set("auto_clear", ...) returns an error.

delete('all');
set("auto_clear", "on");
a1 = gca();
assert_checkequal(a1.auto_clear, "on");
scf();
a2 = gca();
assert_checkequal(a2.auto_clear, "off");
