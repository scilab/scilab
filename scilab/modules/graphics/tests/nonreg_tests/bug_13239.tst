// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13239 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13239
//
// <-- Short Description -->
// Grid not shown in zoomed log scale

data = [1,10];
plot2d(data, data, 2)
a = gca();
a.log_flags = "ll";
a.grid = [color("black"), color("black")];
a.x_ticks = tlist(["ticks", "locations", "labels"], data, string(data));
a.y_ticks = tlist(["ticks","locations", "labels"], data, string(data));
a.sub_ticks = [8 8];

// after the zoom the grid must be always drawn
zoom_rect([2 2 8 8])
