// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// Performs Torczon test cases
//
// Penalty #1 function
//------------------------------
// n = 4
// initial guess = [1 2 3 4]
// finitial  = 885.063
// xoptimum [0.2500075 0.2500075 0.2500075 0.2500075]
// foptimum = 2.249978e-005
//------------------------------
// n = 8
// initial guess = [1 2 3 4 5 6 7 8]
// finitial  = 41514.063
// xoptimum = [0.1767849 0.1767849 0.1767849 0.1767849 0.1767849 0.1767849 0.1767849 0.1767849]
// foptimum = 5.421519e-005
//------------------------------
// n = 10
// initial guess = [1 2 3 4 5 6 7 8 9 10]
// finitial  = 148032.56535
// xoptimum = ?
// foptimum = 7.08765e-5
function [ y , index , n ] = penalty1 ( x , index , n )
  y = 0.0
  for i = 1 : n
    fi = (x(i) - 1) * sqrt(1.e-5)
    y = y + fi^2
  end
  fi = -1/4 + norm(x)^2
  y = y + fi^2
endfunction

function y = penalty1_der ( x , n )
  [ y , index ] = penalty1 ( x , 1 , n )
endfunction

//
// Torczon's termination criteria
function [ this , terminate , status ] = mystoppingrule ( this , simplex )
  global _DATA_
  v1 = optimsimplex_getx ( simplex , 1 )
  delta = max ( 1.0 , norm(v1) )
  maxnorms = 0.0
  n = neldermead_cget ( this , "-numberofvariables" )
  for i = 2 : n
    vi = optimsimplex_getx ( simplex , i )
    ni = norm ( vi - v1 )
    maxnorms = max ( maxnorms , ni )
  end
  epsilon = _DATA_.epsilon
  if ( maxnorms / delta < epsilon ) then
    terminate = %t
    status = "torczon"
  else
    terminate = %f
  end
endfunction
//
// solvepb --
//   Find the solution for the given problem.
// Arguments
//   n : number of variables
//   cfun : cost function 
//   x0 : initial guess
//
function [nbfevals , niter , fopt , cputime ] = solvepb ( n , cfun , x0 , tolerance )
  tic();
  global _DATA_;
  _DATA_ = tlist ( [
    "T_TORCZON"
    "epsilon"
  ]);
  _DATA_.epsilon = tolerance;
  
  nm = neldermead_new ();
  nm = neldermead_configure(nm,"-numberofvariables",n);
  nm = neldermead_configure(nm,"-function",cfun);
  nm = neldermead_configure(nm,"-costfargument",n);
  nm = neldermead_configure(nm,"-x0",x0);
  nm = neldermead_configure(nm,"-maxiter",10000);
  nm = neldermead_configure(nm,"-maxfunevals",10000);
  nm = neldermead_configure(nm,"-tolxmethod",%f);
  nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
  // Turn ON my own termination criteria
  nm = neldermead_configure(nm,"-myterminate",mystoppingrule);
  nm = neldermead_configure(nm,"-myterminateflag",%t);
  //
  // Perform optimization
  //
  nm = neldermead_search(nm);
  niter = neldermead_get ( nm , "-iterations" );
  nbfevals = neldermead_get ( nm , "-funevals" );
  fopt = neldermead_get ( nm , "-fopt" );
  xopt = neldermead_get ( nm , "-xopt" );
  status = neldermead_get ( nm , "-status" );
  nm = neldermead_destroy(nm);
  cputime = toc();
  // Compute angle between gradient and simplex direction
  sopt = neldermead_get ( nm , "-simplexopt" )
  xhigh = optimsimplex_getx ( sopt , n + 1 )
  xbar = optimsimplex_xbar ( sopt )
  d = xbar - xhigh;
  g = derivative ( list ( penalty1_der , n ) , xopt , order=4 );
  cost = -g*d.' / norm(g) / norm(d)
  theta =acosd(cost)
  // Compute condition of matrix of directions
  D = optimsimplex_dirmat ( sopt )
  k = cond ( D )
  // Display result
  mprintf ( "=============================\n")
  mprintf ( "status = %s\n" , status )
  mprintf ( "Tolerance=%e\n" , tolerance )
  mprintf ( "xopt = [%s]\n" , strcat(string(xopt)," ") )
  mprintf ( "fopt = %e\n" , fopt )
  mprintf ( "niter = %d\n" , niter )
  mprintf ( "nbfevals = %d\n" , nbfevals )
  mprintf ( "theta = %25.15f (deg)\n" , theta )
  mprintf ( "cputime = %f (s)\n" , cputime )
  mprintf ( "cond(D) = %e (s)\n" , k )
endfunction

// Solve Penalty #1 with n=4
//x0 = [1 2 3 4].';
//tolerance = 1.e-16;
//[nbfevals , niter , fopt , cputime ] = solvepb ( 4 , penalty1 , x0 , tolerance );

// Solve Penalty #1 with n=8
x0 = [1 2 3 4 5 6 7 8].';
for tol = [1.e-1 1.e-2 1.e-3 1.e-4 1.e-5 1.e-6 1.e-7]
  [nbfevals , niter , fopt , cputime ] = solvepb ( 8 , penalty1 , x0 , tol );
end

// Try with optim
function [ f , g , index ] = penalty1_optim ( x , index , n )
  [ f , index , n ] = penalty1 ( x , 1 , n )
  g = derivative ( list ( penalty1_der , n ) , x , order=4 )
endfunction

function [fopt , cputime ] = solvepb_optim ( n , cfun , x0 , tolerance )
  global _DATA_;
  _DATA_ = tlist ( [
  "T_TORCZON"
  "n"
  ]);
  _DATA_.n = n;
  tic();
  [ fopt , xopt ] = optim ( list ( penalty1_optim , n ) , x0 , "ar" , 100 , 100 , tolerance )
  cputime = toc();
  mprintf ( "=============================\n")
  mprintf ( "Tolerance=%e\n" , tolerance )
  mprintf ( "xopt = [%s]\n" , strcat(string(xopt)," ") )
  mprintf ( "fopt = %e\n" , fopt )
  mprintf ( "cputime = %f (s)\n" , cputime )
endfunction

//x0 = [1 2 3 4].';
//for tol = [1.e-1 1.e-2 1.e-4 1.e-6 1.e-8 1.e-10 1.e-12 1.e-14 1.e-16]
//  [fopt , cputime ] = solvepb_optim ( 4 , penalty1 , x0 , tol );
//end


x0 = [1 2 3 4 5 6 7 8].';
for tol = [1.e-1 1.e-2 1.e-4 1.e-6 1.e-8 1.e-10 1.e-12 1.e-14 1.e-16]
  [fopt , cputime ] = solvepb_optim ( 8 , penalty1 , x0 , tol );
end

