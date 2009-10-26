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

//
//  Reference:
//
//    An extension of the simplex method to constrained
//    nonlinear optimization
//    M.B. Subrahmanyam
//    Journal of optimization theory and applications
//    Vol. 62, August 1989
//
//    Gould F.J.
//    Nonlinear Tolerance Programming
//    Numerical methods for Nonlinear optimization
//    Edited by F.A. Lootsma, pp 349-366, 1972

//
// optimtestcase --
//   Non linear inequality constraints are positive.
//    
// Arguments
//   x: the point where to compute the function
//   index : what to compute
//     if index=2, returns f
//     if index=5, returns c
//     if index=6, returns f and c
// Note
//  The inequality constraints are expected to be positive.
//
function [ f , c , index ] = optimtestcase ( x , index )
  f = []
  c = []
  if ( ( index == 2 ) | ( index == 6 ) ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( ( index == 5 ) | ( index == 6 ) ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction

//
// Test with Box algorithm and default axes initial simplex
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",400);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-4);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
nm = neldermead_destroy(nm);

//
// Test with Box algorithm and restart
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-1);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_search(nm);
nm = neldermead_restart(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "maxfuneval" );
nm = neldermead_destroy(nm);

//
// Test with Box algorithm and default axes initial simplex
// Add bounds and simplex initial length so that there is a need 
// for variable projection.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",400);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-4);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-3 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
nm = neldermead_destroy(nm);
//
// Test with Box algorithm and randomized bounds simplex.
// Add bounds and simplex initial length so that there is a need 
// for variable projection.
// Here the initial simplex is computed with Box randomized bounds method
// and default number of points in the simplex, i.e. 2n = 2 * 4 = 8.
//
// The convergence is not accurate in this case, whatever the 
// value of the relative tolerance on simplex size.
//
//
// Initialize the random number generator, so that the results are always the
// same.
//
rand("seed" , 0)
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-8);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-0 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-1 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "maxfuneval" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_equal ( nbve , 8 );
nm = neldermead_destroy(nm);


//
// Test with Box algorithm and randomized bounds simplex.
// Add bounds and simplex initial length so that there is a need 
// for variable projection.
// Here the initial simplex is computed with Box randomized bounds method
// and user-defined number of points in the simplex, i.e. 6
//
//
// Initialize the random number generator, so that the results are always the
// same.
//
rand("seed" , 0)
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-6);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_configure(nm,"-boxnbpoints",6);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_equal ( nbve , 6 );
nm = neldermead_destroy(nm);
//
// Test with Box algorithm and given simplex.
// Add bounds and simplex initial length so that there is a need 
// for variable projection.
// Here the initial simplex is user-defined.
// Makes sure that all auxiliary computations are performed.
// I put the solution as the last point, to see what happens
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-3);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0method","given");
coords = [
0.0 0.0 0.0 0.0
1.0 0.0 0.0 0.0
0.0 1.0 0.0 0.0
0.0 0.0 1.0 0.0
0.0 0.0 0.0 1.0
1.0 1.0 1.0 1.0
0.0 1.0 2.0 -1.0
];
nm = neldermead_configure(nm,"-coords0",coords);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-2 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-4 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_equal ( nbve , 7 );
nm = neldermead_destroy(nm);

//
// Test with Box algorithm and randomized bounds simplex.
// Test that verbose mode works fine.
//
rand("seed" , 0)
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",5);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-3);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_configure(nm,"-coords0",coords);
nm = neldermead_search(nm);
nm = neldermead_destroy(nm);

