// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Pierre LANDO
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7990 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7990
//
// <-- Short Description -->
// Box thickness was badly exported in vectorial formats (svg/pdf/...)
// Thanks to Paul Griffiths for the patch.


// Run this script and see if the box thickness in the exported file is constant.

scf(); x = [0:0.1:2*%pi]; plot(x, sin(x)); a = gca(); a.thickness = 5;
xs2pdf( gcf(), 'TMPDIR/test.pdf' )

