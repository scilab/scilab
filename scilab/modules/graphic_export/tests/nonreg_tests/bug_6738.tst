// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6738 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6738
//
// <-- Short Description -->
// The horizontal line of the square root symbol was not drawn when exported.

xtitle('$\sqrt{abcdefgh}$');
xs2pdf(0,'export.pdf');