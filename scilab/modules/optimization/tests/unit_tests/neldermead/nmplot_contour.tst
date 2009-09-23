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
// Test a basic contour plot
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
[nm , xdata , ydata , zdata ] = neldermead_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
contour ( xdata , ydata , zdata , 20 )
nm = neldermead_destroy(nm);

//
// Test a vectorized contour plot
//
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

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
// Vectorize the function so that the contour plot is faster
nm = neldermead_configure(nm,"-vectorizefunction",1);
nm = neldermead_configure(nm,"-function",rosenbrockVectorized);
[nm , xdata , ydata , zdata ] = neldermead_contour ( nm , xmin = -2.0 , xmax = 2.0 , ymin = -2.0 , ymax = 2.0 , nx = NP , ny = NP );
contour ( xdata , ydata , zdata , 20 )
nm = neldermead_destroy(nm);


