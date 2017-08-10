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

//
// Check behaviour with default settings.
//

function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_search(nm, "off");
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0;1.0], 1e-4 );
// Cleanup
nm = neldermead_destroy(nm);
clear nm;

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
// Test wrong -restartdetection
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
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-kelleystagnationflag",%t);
nm = neldermead_configure(nm,"-restartflag",%t);
instr = "nm = neldermead_configure(nm,""-restartdetection"",""foo"");";
alloptions = """oneill"" or ""kelley""";
assert_checkerror(instr,"%s: Expected value [%s] for input argument %s at input #%d, but got ""%s"" instead.",[],..
  "neldermead_configure",alloptions,"value",3,"foo");
nm = neldermead_destroy(nm);
