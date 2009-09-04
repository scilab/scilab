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
function f = mckinnon1 ( x )

  if ( length ( x ) ~= 2 )
    error ( 'Error: function expects a two dimensional input\n' );
  end

  tau = 1.0;
  theta = 15.0;
  phi = 10.0;

  if ( x(1) <= 0.0 )
    f = theta * phi * abs ( x(1) ).^tau + x(2) * ( 1.0 + x(2) );
  else
    f = theta       *       x(1).^tau   + x(2) * ( 1.0 + x(2) );
  end
endfunction
function f = mckinnon2 ( x )

  if ( length ( x ) ~= 2 )
    error ( 'Error: function expects a two dimensional input\n' );
  end

  tau = 2.0;
  theta = 6.0;
  phi = 60.0;

  if ( x(1) <= 0.0 )
    f = theta * phi * abs ( x(1) ).^tau + x(2) * ( 1.0 + x(2) );
  else
    f = theta       *       x(1).^tau   + x(2) * ( 1.0 + x(2) );
  end
endfunction
function f = mckinnon3 ( x )

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

