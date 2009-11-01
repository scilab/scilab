// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


mprintf("Illustrates that the variable-shape Nelder-Mead algorithm performs well on a quadratic test case.\n");
mprintf("Defining quadratic function...\n");
function [ y , index ] = quadratic ( x , index )
  y = x(1)^2 + x(2)^2 - x(1) * x(2);
endfunction

mprintf("Creating nmplot object...\n");
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",quadratic);
nm = nmplot_configure(nm,"-x0",[2.0 2.0]');
nm = nmplot_configure(nm,"-maxiter",100);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolxmethod",%f);
nm = nmplot_configure(nm,"-tolsimplexizerelative",1.e-8);
nm = nmplot_configure(nm,"-simplex0method","spendley");
//
// Setup output files
//
nm = nmplot_configure(nm,"-simplexfn",TMPDIR + "\history.simplex.txt");
nm = nmplot_configure(nm,"-fbarfn",TMPDIR + "\history.fbar.txt");
nm = nmplot_configure(nm,"-foptfn",TMPDIR + "\history.fopt.txt");
nm = nmplot_configure(nm,"-sigmafn",TMPDIR + "\history.sigma.txt");
//
// Perform optimization
//
mprintf("Searching (please wait)...\n");
nm = nmplot_search(nm);
nmplot_display(nm);
// Plot various histories
mprintf("Plotting history of fbar...\n");
f = scf();
nmplot_historyplot ( nm , TMPDIR + "\history.fbar.txt" , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
mprintf("Plotting history of fopt...\n");
f = scf();
nmplot_historyplot ( nm , TMPDIR + "\history.fopt.txt" , ...
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
nmplot_historyplot ( nm , TMPDIR + "\history.sigma.txt" , ...
  mytitle = "Logarithm Maximum Oriented length" , myxlabel = "Iterations" );
f.children.log_flags = "nln";
f.children.y_ticks = newticks;
f.children.children(1).children.mark_mode = "on";
f.children.children(1).children.mark_style = 9;
// Plot the contours of the cost function and the simplex history
mprintf("Plotting contour (please wait)...\n");
nm = nmplot_configure(nm,"-verbose",0);
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -2.0 , xmax = 4.0 , ymin = -2.0 , ymax = 4.0 , nx = 50 , ny = 50 );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , [0.1 1.0 2.0 5.0 10.0 15.0 20.0] )
nmplot_simplexhistory ( nm );
drawnow();
// Clean-up
deletefile(TMPDIR + "\history.simplex.txt");
deletefile(TMPDIR + "\history.fbar.txt");
deletefile(TMPDIR + "\history.fopt.txt");
deletefile(TMPDIR + "\history.sigma.txt");
nm = nmplot_destroy(nm);
mprintf("End of demo.\n");

//
// Load this script into the editor
//
filename = 'nmplot_quadratic.variable.sce';
dname = get_absolute_file_path(filename);
editor ( dname + filename );

