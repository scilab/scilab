// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13462 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13462
//
// <-- Short Description -->
// Low levels functions modified bounds even with auto_scale="off"

x = [0; 1]; y = x;
f = scf();
a = f.children;
xpoly(x, y, "lines")
assert_checkequal(a.data_bounds(:, 1), [0 1]');
a.auto_scale = "off";
xpoly(x+0.5, y, "lines");
assert_checkequal(a.data_bounds(:, 1), [0 1]');