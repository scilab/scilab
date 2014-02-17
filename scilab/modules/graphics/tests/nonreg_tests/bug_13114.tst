// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13114 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13114
//
// <-- Short Description -->
// Saving and loading handles before and after the removal of "pixmap"

cd SCI/modules/graphics/tests/nonreg_tests/;

load("bug_13114_1.sod");

load("bug_13114_2.sod");
