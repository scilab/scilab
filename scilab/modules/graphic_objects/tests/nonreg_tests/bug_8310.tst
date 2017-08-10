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

clf();
X = [0 10 10 7  6.5 3.5  3 0 0]';
Y = [0 0 10 10  2 2  10 10 0]';
plot3d(X,Y,zeros(X));

//the first plot should look like:
// -------------------
// |                  |
// |                  |
// |     --------     |
// |     |      |     |
// |     |      |     |
// |     |      |     |
// -------      -------

//---------------------------------------------------------------
x = [0; 5; 10; 5; 0]
y = [0; 10; 0; 5; 0];
scf()
plot3d(x,y,zeros(x));

//The second plot shoul look line an arrow head, not a triangle.
//---------------------------------------------------------------

n =  rand(1,3) - 0.5;
n = n / norm(n);

c = 10 * (rand(1,3) - 0.5);

dx = [0, 5, 10, 15, 20, 15, 10, 5, 0]'
dy = [0, 15, 5, 20, 10, 0, 2, 0, 0]'
dz = zeros(dx)


a = [dx, dy, dz];

b = a * n'

dz = dz - b / n(3)


dx = dx + c(1);
dy = dy + c(2);
dz = dz + c(3);
//add noise
dz = dz + 10*(rand(dz) -0.5);

scf()
plot3d(dx, dy, dz);
scf()
plot(dx,dy);
//Third plot viewed from normal should looks like the 2d plot  from dx,dy with noise on z axis.

