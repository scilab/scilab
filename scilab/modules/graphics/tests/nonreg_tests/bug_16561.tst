// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 16561 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16561
//
// <-- Short Description -->
// histplot(linspace(-10,10,100), rand(5)) yielded an error

assert_checkequal(execstr("histplot(linspace(-10,10,100), rand(5))","errcatch"), 0);
