// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Caio SOUZA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 8310 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8310
//
// <-- Short Description -->
// plot3d was drawing weird triangles when ploting non-convex polygons

X = [0 10 10 7  6.5 3.5  3 0 0]';
Y = [0 0 10 10  2 2  10 10 0]';
clf, plot3d(X($:-1:1),Y($:-1:1),zeros(X));

//the first plot should look like:
// --------------------
// |                  |
// |                  |
// |     --------     |
// |     |      |     |
// |     |      |     |
// |     |      |     |
// -------      -------


x = [0; 5; 10; 5; 0]
y = [0; 10; 0; 5; 0];
scf()
plot3d(x,y,zeros(x));

//The second plot shoul look line an arrow head, not a triangle.
