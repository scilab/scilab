// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1947 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1947
//
// <-- Short Description -->
// Data bounds approximation may be wrong and contour may not fit right
// within the axes

// create a contour
contour(1:5,1:10,ones(5,10),5)

// check that the contour fits well in the axes, that there is no gap
// between axes and the contour lines.

