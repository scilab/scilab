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

//
// Test print with all data here.
//
simplex = [...
    0.    0.  
    1.    0.  
    0.    2.  
];
s1 = optimsimplex_new ();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve ( s1 , 3 );
s1 = optimsimplex_setallx ( s1 , simplex );
s1 = optimsimplex_setallfv ( s1 , [12.0 13.0 14.0]' );
optimsimplex_print ( s1 );
s1 = optimsimplex_destroy ( s1 );

//
// Test print with no data
//
s1 = optimsimplex_new ();
optimsimplex_print ( s1 );
s1 = optimsimplex_destroy ( s1 );
//
// Test print with n set, but no data
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn ( s1 , 2 );
optimsimplex_print ( s1 );
s1 = optimsimplex_destroy ( s1 );
//
// Test print with n set, x set, but no function values
//
s1 = optimsimplex_new ();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve ( s1 , 3 );
s1 = optimsimplex_setallx ( s1 , simplex );
optimsimplex_print ( s1 );
s1 = optimsimplex_destroy ( s1 );

