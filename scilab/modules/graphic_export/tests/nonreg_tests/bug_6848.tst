// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando <pierre.lando@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6848 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6738
//
// <-- Short Description -->
// The patterns of the lines was poorly exported

// Execute this script.

plot(1:1:5 ,'r--');
plot(2:1:6 ,'r-.');
xs2pdf(gcf(),'out.pdf');

// And look if the exported figure is correct.
