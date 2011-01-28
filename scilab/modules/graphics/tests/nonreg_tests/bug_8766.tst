// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8766 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8766
//
// <-- Short Description -->
// Depending on its input matrix's dimensions (number of rows less than 3),
// Matplot1's rect argument is incorrectly taken into account.
//

a = gca();
a.data_bounds = [0,0;10,10];
a.axes_visible = "on";

// With a one-row input matrix, the Matplot object should
// exactly fit to its [4,5,6,8] bounding rectangle.

Matplot1([1,4], [4,5,6,8]);

e = gce();
delete(e);

// It should also exactly fit to its bounding rectangle
// with a 2-row input matrix.

Matplot1([1,4;2,5], [4,5,6,8]);

