// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - pierre.lando@scilab.org
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 1809 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1809
//
// <-- Short Description -->
//   Add warning message (figure_size) after a invalid graphical script

f=gcf()
plot()
f.figure_size=[50000,50000]


// figure_size is not [50000,50000] because limited by your screen resolution.
// A warning message is now displayed.
