// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// Performs O'Neill test cases
//
// Rosenbrock's "banana" function
// initialguess [-1.2 1.0]
// xoptimum [1.0 1.0}
// foptimum 0.0
function [ y , index ] = rosenbrock ( x , index )
y = 100*(x(2)-x(1)^2)^2+(1-x(1))^2;
endfunction
// Powell's quartic valley
// initialguess [3.0 -1.0 0.0 1.0]
// xoptimum [0.0 0.0 0.0 0.0]
// foptimum 0.0
function [ f , index ] = powellquartic ( x , index )
  f = (x(1)+10.0*x(2))^2 + 5.0 * (x(3)-x(4))^2 + (x(2)-2.0*x(3))^4 + 10.0 * (x(1) - x(4))^4
endfunction
// Fletcher and Powell helical valley
// initialguess [-1.0 0.0 0.0]
// xoptimum [1.0 0.0 0.0]
// foptimum 0.0
function [ f , index ] = fletcherpowellhelical ( x , index )
  rho = sqrt(x(1) * x(1) + x(2) * x(2))
  twopi = 2 * %pi
  if ( x(1)==0.0 ) then
    f = 1.e154
  else
    if ( x(1)>0 ) then
      theta = atan(x(2)/x(1)) / twopi
    elseif ( x(1)<0 ) then
      theta = (%pi + atan(x(2)/x(1))) / twopi
    end
    f =  100.0 * (x(3)-10.0*theta)^2 + (rho - 1.0)^2 + x(3)*x(3)
  end
endfunction
// Sum of powers
// initialguess ones(10,1)
// xoptimum zeros(10,1)
// foptimum 0.0
function [ f , index ] = sumpowers ( x , index )
  f = sum(x(1:10).^4);
endfunction

//
// solvepb --
//   Find the solution for the given problem.
// Arguments
//   n : number of variables
//   cfun : cost function 
//   x0 : initial guess
//
function [nbfevals , niter , nbrestart , fopt , cputime ] = solvepb ( n , cfun , x0 )
  tic();
  nm = neldermead_new ();
  nm = neldermead_configure(nm,"-numberofvariables",n);
  nm = neldermead_configure(nm,"-function",cfun);
  nm = neldermead_configure(nm,"-x0",x0);
  nm = neldermead_configure(nm,"-maxiter",1000);
  nm = neldermead_configure(nm,"-maxfunevals",1000);
  nm = neldermead_configure(nm,"-tolxmethod",%f);
  nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
  // Turn ON the tolerance on variance
  nm = neldermead_configure(nm,"-tolvarianceflag",%t);
  nm = neldermead_configure(nm,"-tolabsolutevariance",1.e-16);
  nm = neldermead_configure(nm,"-tolrelativevariance",0.0);
  // Turn ON automatic restart
  nm = neldermead_configure(nm,"-restartflag",%t);
  nm = neldermead_configure(nm,"-restarteps",1.e-3);
  nm = neldermead_configure(nm,"-restartstep",1.0);
  // Turn ON greedy expansion
  nm = neldermead_configure(nm,"-greedy",%t);
  // Set initial simplex to axis-by-axis (this is already the default anyway)
  nm = neldermead_configure(nm,"-simplex0method","axes");
  nm = neldermead_configure(nm,"-simplex0length",1.0);
  nm = neldermead_configure(nm,"-method","variable");
  //nm = neldermead_configure(nm,"-verbose",1);
  //nm = neldermead_configure(nm,"-verbosetermination",1);
  //
  // Perform optimization
  //
  nm = neldermead_search(nm);
  //neldermead_display(nm);
  niter = neldermead_get ( nm , "-iterations" );
  nbfevals = neldermead_get ( nm , "-funevals" );
  fopt = neldermead_get ( nm , "-fopt" );
  xopt = neldermead_get ( nm , "-xopt" );
  nbrestart = neldermead_get ( nm , "-restartnb" );
  status = neldermead_get ( nm , "-status" );
  nm = neldermead_destroy(nm);
  cputime = toc();
  mprintf ( "=============================\n")
  mprintf ( "status = %s\n" , status )
  mprintf ( "xopt = [%s]\n" , strcat(string(xopt)," ") )
  mprintf ( "fopt = %e\n" , fopt )
  mprintf ( "niter = %d\n" , niter )
  mprintf ( "nbfevals = %d\n" , nbfevals )
  mprintf ( "nbrestart = %d\n" , nbrestart )
  mprintf ( "cputime = %f\n" , cputime )
  //mprintf ( "%d %d %e %d %f\n", nbfevals , nbrestart , fopt , niter , cputime );
endfunction

// Solve Rosenbrock's
x0 = [-1.2 1.0].';
[nbfevals , niter , nbrestart , fopt , cputime ] = solvepb ( 2 , rosenbrock , x0 );

// Solve Powell's quartic valley
x0 = [3.0 -1.0 0.0 1.0].';
[nbfevals , niter , nbrestart , fopt , cputime ] = solvepb ( 4 , powellquartic , x0 );

// Solve Fletcher and Powell helical valley
x0 = [-1.0 0.0 0.0].';
[nbfevals , niter , nbrestart , fopt , cputime ] = solvepb ( 3 , fletcherpowellhelical , x0 );

// Solve Sum of powers
x0 = ones(10,1);
[nbfevals , niter , nbrestart , fopt , cputime ] = solvepb ( 10 , sumpowers , x0 );

