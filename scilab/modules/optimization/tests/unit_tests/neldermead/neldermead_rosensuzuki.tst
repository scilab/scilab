// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->




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

function [ f , c , index ] = optimtestcase2 ( x , index )
  f = []
  c = []
  x2 = x.^2
  if ( ( index == 2 ) | ( index == 6 ) ) then
    f = [1 1 2 1]*x2 + [-5 -5 -21 7]*x
  end
  if ( ( index == 5 ) | ( index == 6 ) ) then
    c1 = [-1 -1 -1 -1]*x2 + [-1 1 -1 1]*x + 8
    c2 = [-1 -2 -1 -2]*x2 + [1 0 0 1]*x + 10
    c3 = [-2 -1 -1 0]*x2 + [-2 1 0 1]*x + 5
    c = [c1 c2 c3]
  end
endfunction

//
// Test the function.
//
xstar = [0.0 1.0 2.0 -1.0]';
fstar = -44;
[ f , c , index ] = optimtestcase ( xstar , 6 );
assert_checkequal ( f , fstar );
assert_checkequal ( c , [0 1 0] );
//
[ f , c , index ] = optimtestcase2 ( xstar , 6 );
assert_checkequal ( f , fstar );
assert_checkequal ( c , [0 1 0] );

//
// Test with Box algorithm and default axes initial simplex
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.5 1.0 -0.5]');
nm = neldermead_configure(nm,"-maxiter",400);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-2);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1 , 1.e-1);
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
nm = neldermead_destroy(nm);

//
// Test with Box algorithm and restart
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",300);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-1);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_search(nm);
nm = neldermead_restart(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1, 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "maxfuneval" );
nm = neldermead_destroy(nm);

//
// Test with Box algorithm and default axes initial simplex
// Add bounds and simplex initial length so that there is a need 
// for variable projection.
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",400);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-2);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1, 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-3 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
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
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-2);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-0 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-1 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_checkequal ( nbve , 8 );
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
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-2);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_configure(nm,"-boxnbpoints",6);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1, 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_checkequal ( nbve , 6 );
nm = neldermead_destroy(nm);

//
// Test with "tocenter"
//
rand("seed" , 0)
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-2);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0 -10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0 10.0 10.0]);
nm = neldermead_configure(nm,"-simplex0length",20.0);
nm = neldermead_configure(nm,"-simplex0method","randbounds");
nm = neldermead_configure(nm,"-boxnbpoints",6);
nm = neldermead_configure(nm,"-scalingsimplex0","tocenter");
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-1, 1e-1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-2 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_checkequal ( nbve , 6 );
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
nm = neldermead_configure(nm,"-function",optimtestcase2);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",300);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-2);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
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
assert_checkalmostequal ( xopt , [0.0 1.0 2.0 -1.0]', 1e-2 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , -44.0 , 1e-4 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "tolsize" );
// Check the optimum simplex
simplexopt = neldermead_get ( nm , "-simplexopt" );
nbve = optimsimplex_getnbve ( simplexopt );
assert_checkequal ( nbve , 7 );
nm = neldermead_destroy(nm);

//
// Test with Box algorithm and randomized bounds simplex.
// Test that verbose mode works fine.
//
rand("seed" , 0)
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase2);
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
nm = neldermead_search(nm, "off");
nm = neldermead_destroy(nm);

