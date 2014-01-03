// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13134 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13134
//
// <-- Short Description -->
// User defined ticks in log scale were invisible

data=[1,10];
plot2d(data,data,2)
a = gca();
a.log_flags = "ll";
a.grid = [color("black"),color("black")];
a.x_ticks = tlist(["ticks","locations","labels"],data,string(data));
a.y_ticks = tlist(["ticks","locations","labels"],data,string(data));

// Check ticks in 1, 10 and that there is one subtick on x & y axis.
a.sub_ticks = [1 1];
