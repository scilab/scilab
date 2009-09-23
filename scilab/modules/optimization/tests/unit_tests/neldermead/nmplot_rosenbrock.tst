// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
function y = rosenbrock (x)
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
nm = nmplot_search(nm);
// Plot the contours of the cost function and the simplex history
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , ...
  xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
f = scf();
contour ( xdata , ydata , zdata , 20 )
nmplot_simplexhistory ( nm );
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.fbar.txt" , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.fopt.txt" , ...
  mytitle = "Minimum Function Value" , myxlabel = "Iterations" );
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.sigma.txt" , ...
  mytitle = "Maximum Oriented length" , myxlabel = "Iterations" );
deletefile("rosenbrock.history.simplex.txt");
deletefile("rosenbrock.history.fbar.txt");
deletefile("rosenbrock.history.fopt.txt");
deletefile("rosenbrock.history.sigma.txt");
nm = nmplot_destroy(nm);


// Test Vectorized case with NX = 100 and NY = 100
// nmplot_contour - Vectorized : 1.555
// nmplot_contour - NOT Vectorized : 8.874

//
// rosenbrockVectorized --
//   Compute the rosenbrock function.
// Arguments
//   x : matrix nexp x 2, where nexp is the number of experiments
//
function y = rosenbrockVectorized (x)
  nexp = size(x,1)
  y(1:nexp) = 100*(x(1:nexp,2)-x(1:nexp,1)^2)^2 + (1-x(1:nexp,1))^2;
endfunction


nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
// Vectorize the function so that the contour plot is faster
nm = nmplot_configure(nm,"-vectorizefunction",1);
nm = nmplot_configure(nm,"-function",rosenbrockVectorized);
nm = nmplot_configure(nm,"-x0",[-1.2 1.0].');
nm = nmplot_configure(nm,"-maxiter",10);
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
nm = nmplot_search(nm);
// Plot the contours of the cost function and the simplex history
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , ...
  xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
f = scf();
contour ( xdata , ydata , zdata , 20 )
nmplot_simplexhistory ( nm );
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.fbar.txt" , ...
  mytitle = "Function Value Average" , myxlabel = "Iterations" );
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.fopt.txt" , ...
  mytitle = "Minimum Function Value" , myxlabel = "Iterations" );
f = scf();
nmplot_historyplot ( nm , "rosenbrock.history.sigma.txt" , ...
  mytitle = "Maximum Oriented length" , myxlabel = "Iterations" );
deletefile("rosenbrock.history.simplex.txt");
deletefile("rosenbrock.history.fbar.txt");
deletefile("rosenbrock.history.fopt.txt");
deletefile("rosenbrock.history.sigma.txt");
nm = nmplot_destroy(nm);

