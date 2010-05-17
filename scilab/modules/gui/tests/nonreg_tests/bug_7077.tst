// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7077 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7077
//
// <-- Short Description -->
// uigetfont makes scilab hangs when you clicked directly on cancel

// To save some paper, this test is interactive

fontname = uigetfont();

// Now click on the cancel button.
// Scilab should not hangs.

