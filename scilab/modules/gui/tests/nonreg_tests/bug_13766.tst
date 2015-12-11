// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13766 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13766
//
// <-- Short Description -->
// Setting figure_size property lead to wrong display and figure properties values.

fg = figure();
fg.figure_size = [100,100];
assert_checkequal(fg.figure_size, [100,100]);
