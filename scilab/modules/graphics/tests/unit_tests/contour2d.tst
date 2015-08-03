// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
funcprot(0);
//
// Test #0: the demo
h = scf();
contour ( );
close(h);
//
// Test #1: use a function
function [ f , g , H ] = rosenbrock ( x )
    f = 100.0 *( x (2) - x (1)^2)^2 + (1 - x (1))^2;
endfunction

function f = rosenbrockC ( x1 , x2 )
    f = rosenbrock ( [x1 , x2 ]' )
endfunction

xdata = linspace ( -2 , 2 , 100 );
ydata = linspace ( -1 , 2 , 100 );
h = scf();
contour ( xdata , ydata , rosenbrockC , [2 10 100 500 1000 2000] );
close(h);
//
// Test #2: use a function
t=linspace(-%pi,%pi,30);
function z=my_surface(x, y)
   z=x*sin(x)^2*cos(y)
endfunction

h = scf();
contour(t,t,my_surface,10)
// changing the format of the printing of the levels
xset("fpf","%.1f")
clf()
contour(t,t,my_surface,10)
close(h);
//
// Test #3: use data
h = scf();
z=feval(t,t,my_surface);
contour(t,t,z+0.2*abs(z),20,flag=[0 2 4]);
close(h);

funcprot(1);
