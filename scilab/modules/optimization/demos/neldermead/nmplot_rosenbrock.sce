// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
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
//
// Setup output files
//
simplexfn = TMPDIR + filesep() + "history.simplex.txt";
fbarfn = TMPDIR + filesep() + "history.fbar.txt";
foptfn = TMPDIR + filesep() + "history.fopt.txt";
sigmafn = TMPDIR + filesep() + "history.sigma.txt";
nm = nmplot_configure(nm,"-simplexfn",simplexfn);
nm = nmplot_configure(nm,"-fbarfn",fbarfn);
nm = nmplot_configure(nm,"-foptfn",foptfn);
nm = nmplot_configure(nm,"-sigmafn",sigmafn);
//
// Perform optimization
//
mprintf("Searching (please wait)...\n");
nm = nmplot_search(nm);
nmplot_display(nm);
// Plot the contours of the cost function and the simplex history
mprintf("Plotting contour (please wait)...\n");
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -1.0 , ymax = 2.5 , nx = 50 , ny = 50 );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , [2 10 100 500 1000 2000] )
nmplot_simplexhistory ( nm );
drawnow();
mprintf("Plotting history of fbar...\n");
f = scf();
nmplot_historyplot ( nm , fbarfn, mytitle = "Function Value Average" , myxlabel = "Iterations" );
mprintf("Plotting history of fopt...\n");
f = scf();
nmplot_historyplot ( nm , foptfn, mytitle = "Minimum Function Value" , myxlabel = "Iterations" );
mprintf("Plotting history of sigma...\n");
f = scf();
nmplot_historyplot ( nm , sigmafn, mytitle = "Maximum Oriented length" , myxlabel = "Iterations" );
mprintf("Cleaning up...\n");
deletefile(simplexfn);
deletefile(fbarfn);
deletefile(foptfn);
deletefile(sigmafn);
nm = nmplot_destroy(nm);
mprintf("End of demo.\n");

//
// Load this script into the editor
//
filename = 'nmplot_rosenbrock.sce';
dname = get_absolute_file_path(filename);
editor ( dname + filename );

