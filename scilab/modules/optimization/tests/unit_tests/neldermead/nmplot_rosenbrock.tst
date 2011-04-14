// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

// Use 20 points in X and 20 points in Y
NP = 20;

//
// Check behaviour with default settings.
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
nm = nmplot_configure(nm,"-x0",[-1.2 1.0].');
nm = nmplot_configure(nm,"-maxiter",10);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolfunrelative",10*%eps);
nm = nmplot_configure(nm,"-tolxrelative",10*%eps);
nm = nmplot_configure(nm,"-simplex0method","axes");
nm = nmplot_configure(nm,"-simplex0length",1.0);
nm = nmplot_configure(nm,"-method","variable");
//
// Setup output files
//
nm = nmplot_configure(nm,"-simplexfn",fullfile(TMPDIR,"rosenbrock.history.simplex.txt"));
nm = nmplot_configure(nm,"-fbarfn",fullfile(TMPDIR,"rosenbrock.history.fbar.txt"));
nm = nmplot_configure(nm,"-foptfn",fullfile(TMPDIR,"rosenbrock.history.fopt.txt"));
nm = nmplot_configure(nm,"-sigmafn",fullfile(TMPDIR,"rosenbrock.history.sigma.txt"));
//
// Perform optimization
//
nm = nmplot_search(nm);
// Plot the contours of the cost function and the simplex history
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , ...
  xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , 20 )
nmplot_simplexhistory ( nm );
drawnow();
close(f);
f = scf();
nmplot_historyplot ( nm , fullfile(TMPDIR,"rosenbrock.history.fbar.txt") , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
close(f);
f = scf();
nmplot_historyplot ( nm , fullfile(TMPDIR,"rosenbrock.history.fopt.txt") , ...
  mytitle = "Minimum Function Value" , myxlabel = "Iterations" );
close(f);
f = scf();
nmplot_historyplot ( nm , fullfile(TMPDIR,"rosenbrock.history.sigma.txt") , ...
  mytitle = "Maximum Oriented length" , myxlabel = "Iterations" );
close(f);
deletefile(fullfile(TMPDIR,"rosenbrock.history.simplex.txt"));
deletefile(fullfile(TMPDIR,"rosenbrock.history.fbar.txt"));
deletefile(fullfile(TMPDIR,"rosenbrock.history.fopt.txt"));
deletefile(fullfile(TMPDIR,"rosenbrock.history.sigma.txt"));
nm = nmplot_destroy(nm);



