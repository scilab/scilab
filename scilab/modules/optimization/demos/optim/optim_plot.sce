// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

mprintf("Running optimization...\n");

// 1. Define rosenbrock
function [ f , g , ind ] = rosenbrock ( x , ind )
  if ((ind == 1) | (ind == 4)) then
    f = 100.0 *(x(2)-x(1)^2)^2 + (1-x(1))^2;
  end
  if ((ind == 1) | (ind == 4)) then
    g(1) = - 400. * ( x(2) - x(1)**2 ) * x(1) -2. * ( 1. - x(1) )
    g(2) = 200. * ( x(2) - x(1)**2 )
  end
  if (ind == 1) then
    plot ( x(1) , x(2) , "g." )
  end
endfunction
x0 = [-1.2 1.0];
xopt = [1.0 1.0];
// 2. Draw the contour of Rosenbrock's function
mprintf("Draw contours...\n");
xmin = -2.0
xmax = 2.0
stepx = 0.1
ymin = -1.0
ymax = 2.0
stepy = 0.1
nx = 100
ny = 100
stepx = (xmax - xmin)/nx;
xdata = xmin:stepx:xmax;
stepy = (ymax - ymin)/ny;
ydata = ymin:stepy:ymax;
for ix = 1:length(xdata)
    for iy = 1:length(ydata)
      x = [xdata(ix) ydata(iy)];
      [ f , g , ind ] = rosenbrock ( x , 4 );
      zdata ( ix , iy ) = f;
    end
end
f = scf();
contour ( xdata , ydata , zdata , [1 10 100 500 1000])
plot(x0(1) , x0(2) , "b.")
plot(xopt(1) , xopt(2) , "r*")
// 3. Plot the optimization process, during optimization
mprintf("Plot points during optimization...\n");
[ fopt , xopt ] = optim ( rosenbrock , x0 , imp = -1)
//
// Load this script into the editor
//
filename = 'optim_plot.sce';
dname = get_absolute_file_path(filename);
editor ( dname + filename );

