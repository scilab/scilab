// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3952 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3952
//
// <-- Short Description -->
// legend only works when the number of strings equals the number of polylines
// 

x = 1:10;
plot(x,x);
p1 = gce();
p1 = p1.children(1);
p1.foreground = 3;

plot(x,x+1);
p2 = gce();
p2 = p2.children(1);
p2.foreground = 4;

plot(x,x+2);
p3 = gce();
p3 = p3.children(1);
p3.foreground = 5;

// only two legends
legend(["polyline 1", "polyline 2"]);
