// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

mprintf("Defining Rosenbrock function...\n");
function [ y , index ] = rosenbrock ( x , index )
y = 100*(x(2)-x(1)^2)^2+(1-x(1))^2;
endfunction
x0 = [-1.2 1.0]';
mprintf("x0=%s\n",strcat(string(x0)," "));
mprintf("Creating object...\n");
nm = neldermead_new ();
mprintf("Configuring object...\n");
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
mprintf("Searching for minimum...\n");
nm = neldermead_search(nm);
neldermead_display(nm);
mprintf("Plot contour...\n");
xmin = -2.0 ; 
xmax = 2.0 ; 
ymin = -2.0 ; 
ymax = 2.0 ; 
nx = 50; 
ny = 50;
stepx = (xmax - xmin)/nx
xdata = xmin:stepx:xmax;
stepy = (ymax - ymin)/ny
ydata = ymin:stepy:ymax;
for ix = 1:length(xdata)
  for iy = 1:length(ydata)
    experiment = [xdata(ix) ydata(iy)];
    [ nm , fiexp ] = neldermead_function ( nm , experiment );
    zdata ( ix , iy ) = fiexp;
  end
end
wnum = 100001;
my_handle             = scf(wnum);
contour ( xdata , ydata , zdata , [3 10 100 500 1000 2000] )
// Plot starting point
mprintf("x0 : blue dot\n");
plot(x0(1),x0(2));
my_handle.children.children(1).children.mark_mode="on";
my_handle.children.children(1).children.mark_size = 5;
my_handle.children.children(1).children.mark_foreground = 2;
mprintf("xopt : red dot\n");
xopt = neldermead_get(nm,"-xopt");
plot(xopt(1),xopt(2));
my_handle.children.children(1).children.mark_mode="on";
my_handle.children.children(1).children.mark_size = 5;
my_handle.children.children(1).children.mark_foreground = 5;

nm = neldermead_destroy(nm);
clear nm;


