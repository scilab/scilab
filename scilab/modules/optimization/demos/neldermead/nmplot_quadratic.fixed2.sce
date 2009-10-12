// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


mprintf("Illustrates that the fixed-shape Spendley et al. algorithm performs badly on a badly quadratic test case.\n");
mprintf("Defining quadratic function...\n");
a = 100;
function [ y , index ] = quadratic ( x , index )
  y = a * x(1)^2 + x(2)^2;
endfunction

mprintf("Creating nmplot object...\n");
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",quadratic);
nm = nmplot_configure(nm,"-x0",[10.0 10.0]');
nm = nmplot_configure(nm,"-maxiter",400);
nm = nmplot_configure(nm,"-maxfunevals",400);
nm = nmplot_configure(nm,"-tolxmethod",%f);
nm = nmplot_configure(nm,"-tolsimplexizerelative",1.e-8);
nm = nmplot_configure(nm,"-simplex0method","spendley");
nm = nmplot_configure(nm,"-method","fixed");
//nm = nmplot_configure(nm,"-verbose",1);
//nm = nmplot_configure(nm,"-verbosetermination",0);
//
// Setup output files
//
nm = nmplot_configure(nm,"-simplexfn","rosenbrock.fixed.history.simplex.txt");
nm = nmplot_configure(nm,"-fbarfn","rosenbrock.fixed.history.fbar.txt");
nm = nmplot_configure(nm,"-foptfn","rosenbrock.fixed.history.fopt.txt");
nm = nmplot_configure(nm,"-sigmafn","rosenbrock.fixed.history.sigma.txt");
//
// Perform optimization
//
mprintf("Searching for minimum...\n");
nm = nmplot_search(nm);
nmplot_display(nm);
// Plot various histories
mprintf("Plotting history of fbar...\n");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.fixed.history.fbar.txt" , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
mprintf("Plotting history of fopt...\n");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.fixed.history.fopt.txt" , ...
  mytitle = "Logarithm Minimum Function Value" , myxlabel = "Iterations" );
f.children.log_flags = "nln";
newticks = tlist(["ticks","locations","labels"]);
newticks.labels = ["1.e-20" "1.e-10" "1.e-1"];
newticks.locations = [1.e-20 1.e-10 1.e-1];
f.children.y_ticks = newticks;
f.children.children(1).children.mark_mode = "on";
f.children.children(1).children.mark_style = 9;
mprintf("Plotting history of sigma...\n");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.fixed.history.sigma.txt" , ...
  mytitle = "Logarithm Maximum Oriented length" , myxlabel = "Iterations" );
f.children.log_flags = "nln";
f.children.y_ticks = newticks;
f.children.children(1).children.mark_mode = "on";
f.children.children(1).children.mark_style = 9;
// Plot the contours of the cost function and the simplex history
mprintf("Plotting contour...\n");
nm = nmplot_configure(nm,"-verbose",0);
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -5.0 , xmax = 12.0 , ymin = -2.0 , ymax = 12.0 , nx = 50 , ny = 50 );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , [10.0 50 100 1000 2000 5000 10000 20000] )
nmplot_simplexhistory ( nm );
drawnow();
// Clean-up
deletefile("rosenbrock.fixed.history.simplex.txt");
deletefile("rosenbrock.fixed.history.fbar.txt");
deletefile("rosenbrock.fixed.history.fopt.txt");
deletefile("rosenbrock.fixed.history.sigma.txt");
nm = nmplot_destroy(nm);


