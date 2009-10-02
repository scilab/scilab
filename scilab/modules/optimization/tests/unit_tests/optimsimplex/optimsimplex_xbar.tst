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
// iexcl by default
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 );
assert_close ( cen , [-1.2 1.5], %eps );
s1 = optimsimplex_destroy ( s1 );

// Case iexcl = 2
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 , 2 );
assert_close ( cen , [-0.7 1.0], %eps );
s1 = optimsimplex_destroy ( s1 );

// Case iexcl = 2:3
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 , 2:3 );
assert_close ( cen , [-1.2 1.0], %eps );
s1 = optimsimplex_destroy ( s1 );
//
// Test with 5 vertices and default exclusion
//
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
93.6 0.0 0.0
93.6 10.0 10.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 );
assert_close ( cen , [-0.65 1.0], %eps );
s1 = optimsimplex_destroy ( s1 );

//
// Test with 5 vertices and several exclusions
//
s1 = optimsimplex_new ();
simplex = [
24.2 -1.2 1.0
36.2 -1.2 2.0
93.6 -0.2 1.0
93.6 0.0 0.0
93.6 10.0 10.0
];
s1 = optimsimplex_setall ( s1 , simplex );
cen = optimsimplex_xbar ( s1 , [1 3 5]);
assert_close ( cen , [-0.6    1.0], 10 * %eps );
s1 = optimsimplex_destroy ( s1 );

