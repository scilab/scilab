// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

mprintf("Illustrates the 2nd counter example given by Han et al.\n");

//
// han2 --
//   Compute the cost function from the Han Phd Thesis
//   which exhibits a failure of the NM method.
// Reference 
//   Algorithms in Unconstrained Optimization
//   Han, Lixing
//   2000
//   Ph.D., The University of Connecticut
//
function [ f , index ] = han2 ( x , index )
  if abs(x(2)) <= 1.0 then
    rho = 0.0
  elseif x(2) > 1.0 then
    rho = x(2) - 1
  else
    rho = -x(2) - 1
  end
  f = x(1)^2 + rho;
endfunction


coords0 = [
    0.    0.5  
    0.   -0.5  
    1.    0.   
]


mprintf("Creating nmplot object...\n");
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",han2);
nm = nmplot_configure(nm,"-x0",[1.0 1.0]');
nm = nmplot_configure(nm,"-maxiter",50);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolfunrelative",10*%eps);
nm = nmplot_configure(nm,"-tolxrelative",10*%eps);
nm = nmplot_configure(nm,"-simplex0method","given");
nm = nmplot_configure(nm,"-coords0",coords0);
nm = nmplot_configure(nm,"-simplex0length",1.0);
nm = nmplot_configure(nm,"-method","variable");
//nm = nmplot_configure(nm,"-verbose",1);
nm = nmplot_configure(nm,"-verbosetermination",1);
//
// Setup output files
//
nm = nmplot_configure(nm,"-simplexfn","han2-history-simplex.txt");
//
// Perform optimization
//
mprintf("Searching for minimum...\n");
nm = nmplot_search(nm);
nmplot_display(nm);
//
// Plot
//
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -0.2 , xmax = 1.2 , ymin = -1.5 , ymax = 1.5 , nx = 50 , ny = 50 );
f = scf(100001);
xset("fpf"," ")
drawlater();
contour ( xdata , ydata , zdata , [0.1 0.2 0.5 1.0 1.5 1.9] )
nmplot_simplexhistory ( nm );
drawnow();
deletefile("han2-history-simplex.txt");
nm = nmplot_destroy(nm);


