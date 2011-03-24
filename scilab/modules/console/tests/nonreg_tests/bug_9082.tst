// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 9082 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9082
//
// <-- Short Description -->
// It was not possible to preview LaTeX when it was a second LaTeX string.

a=["$\frac\pi2$"; "$\int_0^\inftyf(x)\mathrm{d}x$"];

// Move the caret on the first and on the second LaTeX string
// The LaTeX preview should be active on the two strings.