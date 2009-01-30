// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1849 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1849
//
// <-- Short Description -->
// The field line_mode of handle Legend does not work.
// 

// plot something
plot(1:10);

// add a legend
leg = legend("My plot");
leg.line_mode = "off";
leg.line_mode = "on";

// check the line around the legend
