// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15638 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15638
//
// <-- Short Description -->
// colorbar() displayed tips colors twice smaller than for other ones

x = linspace(0,1,81);
z = cos(2*%pi*x)'*sin(2*%pi*x);
clf()
gcf().color_map = jetcolormap(4);
Sgrayplot(x, x, z);
colorbar(min(z), max(z));
contour(x,x,z,[-0.5 0 0.5]);
gce().children.children(1:2:$-1).foreground=-1; // contours in black

// CHECKING:
// * The bar must have graduations from -1 to 1
// * The 4 colors must all span over 0.25
// * On the plot,
//   the 0.5 contour must be exactly at the red/yellow limit, as on the bar
//   the 0.0 contour must be exactly at the cyan/yellow limit, as on the bar
//   the -0.5 contour must be exactly at the blue/cyan limit, as on the bar


function z = f(x,y)
    z = cos(2*%pi*x)'*sin(2*%pi*y);
endfunction
clf()
gcf().color_map = jetcolormap(4);
x = linspace(0,1,81);
Sfgrayplot(x, x, f);
z = f(x,x);
colorbar(min(z), max(z));
contour(x,x,z,[-0.5 0 0.5]);
gce().children.children(1:2:$-1).foreground=-1; // contours in black

// CHECKING: AS ABOVE


