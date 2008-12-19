// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 3844 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3844
//
// <-- Short Description -->
// axes with a very high y-bounds/x-bounds ratio do not appear

// the plot should appear
// could be easily checked with some image processing
plot2d([1 2], [1 2]*1e19);

