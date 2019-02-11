// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7629 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7629
//
// <-- Short Description -->
// Browsevar is not live-updated when a variable is cleared.
// Then double-clicking on a cleared variable misworks (as expected).

a = 1;
browsevar();
clear a;
// In the browser, the "a" should no longer be listed.
