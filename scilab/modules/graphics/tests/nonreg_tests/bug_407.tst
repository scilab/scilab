// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 407 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=407
//
// <-- Short Description -->
// Polylines with thickness greater than 2 and with more than 1380
// points are not displayed

// should complain about strf (normally a strig of length 3).
xset("thickness",2);
// check if the curve is displayed
plot(1:10000);



