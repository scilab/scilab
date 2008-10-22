// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// non regression test to check that graphic is not diseapearing when zoomed deeply.

// plot something
x = -4:0.1:4;
y = x;
z = cos(x)' * sin(y);
plot3d(x,y,z);

// then zoom deeply (this values used to create a blank window)
axes = gca();
axes.zoom_box = [1.2102359,3.0931702,1.2189472,3.2592903,-0.94,-0.8764004];


