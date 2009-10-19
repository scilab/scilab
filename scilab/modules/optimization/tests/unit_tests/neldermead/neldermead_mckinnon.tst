// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


// <-- ENGLISH IMPOSED -->
// There is a Warning message in the .dia of this test,
// with respect to the conditionning of the direction matrix.
// This is an expected message, because the simplex is degenerated
// after the first optimization, which has failed.
// The restart allows to compute a new simplex and makes the 
// optimization converge toward the good point.

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

//% MCKINNON computes the McKinnon function.
//
//  Discussion:
//
//    This function has a global minimizer:
//
//      X* = ( 0.0, -0.5 ), F(X*) = -0.25
//
//    There are three parameters, TAU, THETA and PHI.
//
//    1 < TAU, then F is strictly convex.
//             and F has continuous first derivatives.
//    2 < TAU, then F has continuous second derivatives.
//    3 < TAU, then F has continuous third derivatives.
//
//    However, this function can cause the Nelder-Mead optimization
//    algorithm to "converge" to a point which is not the minimizer
//    of the function F.
//
//    Sample parameter values which cause problems for Nelder-Mead 
//    include:
//
//      TAU = 1, THETA = 15, PHI =  10;
//      TAU = 2, THETA =  6, PHI =  60;
//      TAU = 3, THETA =  6, PHI = 400;
//
//    To get the bad behavior, we also assume the initial simplex has the form
//
//      X1 = (0,0),
//      X2 = (1,1),
//      X3 = (A,B), 
//
//    where 
//
//      A = (1+sqrt(33))/8 =  0.84307...
//      B = (1-sqrt(33))/8 = -0.59307...
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    09 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Ken McKinnon,
//    Convergence of the Nelder-Mead simplex method to a nonstationary point,
//    SIAM Journal on Optimization,
//    Volume 9, Number 1, 1998, pages 148-158.
//
//  Parameters:
//
//    Input, real X(2), the argument of the function.
//
//    Output, real F, the value of the function at X.
//
// Copyright (C) 2009 - INRIA - Michael Baudin, Scilab port

function [ f , index ] = mckinnon3 ( x , index )

  if ( length ( x ) ~= 2 )
    error ( 'Error: function expects a two dimensional input\n' );
  end

  tau = 3.0;
  theta = 6.0;
  phi = 400.0;

  if ( x(1) <= 0.0 )
    f = theta * phi * abs ( x(1) ).^tau + x(2) * ( 1.0 + x(2) );
  else
    f = theta       *       x(1).^tau   + x(2) * ( 1.0 + x(2) );
  end
endfunction

lambda1 = (1.0 + sqrt(33.0))/8.0;
lambda2 = (1.0 - sqrt(33.0))/8.0;
coords0 = [
1.0  1.0
0.0  0.0 
lambda1 lambda2
];

//
// Test with default NM
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_search(nm);
// Check optimum point
// This is not the real, absolute optimum,
// but this is the expected result of the Nelder-Mead
// algorithm.
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;0.0], 1e-6 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , 0.0 , 1e-6 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
nm = neldermead_destroy(nm);

//
// Test with manual restart
// Uses oriented simplex for restart.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_search(nm);
nm = neldermead_restart ( nm );
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;-0.5], 1e-5 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -0.25 , 1e-6 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Check iterations
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations > 200 , %t );
nm = neldermead_destroy(nm);

//
// Test with Kelley stagnation criteria
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-kelleystagnationflag",1);
nm = neldermead_search(nm);
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "kelleystagnation" );
nm = neldermead_destroy(nm);

//
// Test with auto-restart, Kelley stagnation detection and Kelley restart method
// Uses oriented simplex for restart.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-kelleystagnationflag",1);
nm = neldermead_configure(nm,"-restartflag",1);
nm = neldermead_configure(nm,"-restartdetection","kelley");
nm = neldermead_search(nm);
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "maxrestart" );
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;-0.5], 1e-5 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -0.25 , 1e-6 );
// Check iterations
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations > 100 , %t );
// Check number of restarts
restartnb = neldermead_get ( nm , "-restartnb" );
assert_equal ( restartnb , 3 );
nm = neldermead_destroy(nm);

//
// Test with auto-restart, low precision on simplex size and O'Neill restart method.
// Uses oriented simplex for restart.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-tolsimplexizemethod",%t);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-restartflag",1);
nm = neldermead_configure(nm,"-restartdetection","oneill");
nm = neldermead_search(nm);
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;-0.5], 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -0.25 , 1e-4 );
// Check iterations
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations > 40 , %t );
// Check number of restarts
restartnb = neldermead_get ( nm , "-restartnb" );
assert_equal ( restartnb , 2 );
nm = neldermead_destroy(nm);

//
// Test with auto-restart, Kelley stagnation detection and Kelley restart method
// Use axes simplex for restart.
// Use a reduced alpha0 so that restart occur earlier.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-kelleystagnationflag",1);
nm = neldermead_configure(nm,"-restartflag",1);
nm = neldermead_configure(nm,"-restartdetection","kelley");
nm = neldermead_configure(nm,"-restartsimplexmethod","axes");
nm = neldermead_configure(nm,"-kelleystagnationalpha0",1.e-2);
nm = neldermead_search(nm);
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;-0.5], 1e-5 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -0.25 , 1e-4 );
// Check iterations
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations > 50 , %t );
// Check number of restarts
restartnb = neldermead_get ( nm , "-restartnb" );
assert_equal ( restartnb , 2 );
nm = neldermead_destroy(nm);

//
// Test with auto-restart, Kelley stagnation detection and Kelley restart method
// Use spendley simplex for restart.
// Use a reduced alpha0 so that restart occur earlier.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-kelleystagnationflag",1);
nm = neldermead_configure(nm,"-restartflag",1);
nm = neldermead_configure(nm,"-restartdetection","kelley");
nm = neldermead_configure(nm,"-restartsimplexmethod","spendley");
nm = neldermead_configure(nm,"-kelleystagnationalpha0",1.e-2);
nm = neldermead_search(nm);
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;-0.5], 1e-4 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -0.25 , 1e-4 );
// Check iterations
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations > 50 , %t );
// Check number of restarts
restartnb = neldermead_get ( nm , "-restartnb" );
assert_equal ( restartnb , 2 );
nm = neldermead_destroy(nm);

//
// Test with auto-restart, Kelley stagnation detection and Kelley restart method
// Use pfeffer simplex for restart.
// Use a reduced alpha0 so that restart occur earlier.
// Pfeffer's initial simplex is the best for restart, since it 
// respects the optimal point computed so far. It saves tens of iterations.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",mckinnon3);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",500);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-simplex0method","given");
nm = neldermead_configure(nm,"-coords0",coords0);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-kelleystagnationflag",1);
nm = neldermead_configure(nm,"-restartflag",1);
nm = neldermead_configure(nm,"-restartdetection","kelley");
nm = neldermead_configure(nm,"-restartsimplexmethod","pfeffer");
nm = neldermead_configure(nm,"-kelleystagnationalpha0",1.e-2);
nm = neldermead_search(nm);
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "maxrestart" );
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0;-0.5], 1e-5 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -0.25 , 1e-4 );
// Check iterations
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations > 110 , %t );
// Check number of restarts
restartnb = neldermead_get ( nm , "-restartnb" );
assert_equal ( restartnb , 3 );
nm = neldermead_destroy(nm);


