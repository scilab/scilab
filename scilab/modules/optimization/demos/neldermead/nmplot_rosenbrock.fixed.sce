// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

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
nm = nmplot_configure(nm,"-method","fixed");
nm = nmplot_configure(nm,"-verbose",1);
nm = nmplot_configure(nm,"-verbosetermination",0);
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
nm = nmplot_search(nm);
// Plot the contours of the cost function and the simplex history
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = 100 , ny = 100 );
f = scf();
contour ( xdata , ydata , zdata , 20 )
nmplot_simplexhistory ( nm );
xs2png(0,"rosenbrock.fixed.history.simplex.png");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.fixed.history.fbar.txt" , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
xs2png(1,"rosenbrock.fixed.history.fbar.png");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.fixed.history.fopt.txt" , ...
  mytitle = "Minimum Function Value" , myxlabel = "Iterations" );
xs2png(2,"rosenbrock.fixed.history.fopt.png");
f = scf();
nmplot_historyplot ( nm , "rosenbrock.fixed.history.sigma.txt" , ...
  mytitle = "Maximum Oriented length" , myxlabel = "Iterations" );
xs2png(3,"rosenbrock.fixed.history.sigma.png");
deletefile("rosenbrock.fixed.history.simplex.txt");
deletefile("rosenbrock.fixed.history.fbar.txt");
deletefile("rosenbrock.fixed.history.fopt.txt");
deletefile("rosenbrock.fixed.history.sigma.txt");
nm = nmplot_destroy(nm);


