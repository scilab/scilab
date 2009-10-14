// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


mprintf("Illustrates the Nelder-Mead algorithm on Rosenbrock test case.\n");
mprintf("Defining Rosenbrock function...\n");
function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

mprintf("Creating nmplot object...\n");
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
nm = nmplot_configure(nm,"-x0",[-1.2 1.0]');
nm = nmplot_configure(nm,"-maxiter",200);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolfunrelative",10*%eps);
nm = nmplot_configure(nm,"-tolxrelative",10*%eps);
nm = nmplot_configure(nm,"-simplex0method","axes");
nm = nmplot_configure(nm,"-simplex0length",1.0);
nm = nmplot_configure(nm,"-method","variable");
//nm = nmplot_configure(nm,"-verbose",1);
nm = nmplot_configure(nm,"-verbosetermination",0);
//
// Setup output files
//
nm = nmplot_configure(nm,"-simplexfn","rosenbrock.history.simplex.txt");
nm = nmplot_configure(nm,"-fbarfn","rosenbrock.history.fbar.txt");
nm = nmplot_configure(nm,"-foptfn","rosenbrock.history.fopt.txt");
nm = nmplot_configure(nm,"-sigmafn","rosenbrock.history.sigma.txt");
//
// Perform optimization
//
mprintf("Searching for minimum...\n");
nm = nmplot_search(nm);
nmplot_display(nm);
// Plot the contours of the cost function and the simplex history
mprintf("Plotting contour...\n");
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -1.0 , ymax = 2.5 , nx = 50 , ny = 50 );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , [2 10 100 500 1000 2000] )
nmplot_simplexhistory ( nm );
drawnow();
mprintf("Plotting history of fbar...\n");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.fbar.txt" , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
mprintf("Plotting history of fopt...\n");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.fopt.txt" , ...
  mytitle = "Minimum Function Value" , myxlabel = "Iterations" );
mprintf("Plotting history of sigma...\n");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.sigma.txt" , ...
  mytitle = "Maximum Oriented length" , myxlabel = "Iterations" );
mprintf("Cleaning up...\n");
deletefile("rosenbrock.history.simplex.txt");
deletefile("rosenbrock.history.fbar.txt");
deletefile("rosenbrock.history.fopt.txt");
deletefile("rosenbrock.history.sigma.txt");
nm = nmplot_destroy(nm);


