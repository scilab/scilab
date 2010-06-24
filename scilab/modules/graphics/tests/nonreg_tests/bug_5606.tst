// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5606 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5606
//
// <-- Short Description -->
// User-defined ticks labels are misaligned for a bottom-positioned x-axis
// 

f=scf();
a=gca();
plot([0,12],[0,0])
months = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
a.x_ticks = tlist(["ticks", "locations", "labels"], 0.5:11.5, months); a.font_size = 3;

// All the ticks labels' baselines should be aligned

