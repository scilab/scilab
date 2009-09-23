// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// Check behaviour with default settings.
//

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
// Test optimsimplex_size with 3 vertices
//
simplex = [...
    0.    0.  
    1.    0.  
    0.    2.  
];
s1 = optimsimplex_new();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve ( s1 , 3 );
s1 = optimsimplex_setallx ( s1 , simplex );
ssize = optimsimplex_size ( s1 , "Nash" );
assert_close ( ssize , 3.0, %eps );
ssize = optimsimplex_size ( s1 , "diameter" );
assert_close ( ssize , sqrt(5), %eps );
ssize = optimsimplex_size ( s1 , "sigmaplus" );
assert_close ( ssize , 2.0, %eps );
ssize = optimsimplex_size ( s1 , "sigmaminus" );
assert_close ( ssize , 1.0, %eps );
s1 = optimsimplex_destroy(s1);
//
// Test optimsimplex_size with 5 vertices
//
simplex = [...
0.0 0.0 0.0
0.0 1.0 0.0
0.0 0.0 2.0
0.0 1.0 2.0
0.0 2.0 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
ssize = optimsimplex_size ( s1 , "Nash" );
assert_close ( ssize , 10.0 , %eps );
ssize = optimsimplex_size ( s1 , "diameter" );
assert_close ( ssize , 2.8284271247461902909492 , %eps );
ssize = optimsimplex_size ( s1 , "sigmaplus" );
assert_close ( ssize , 2.8284271247461902909492 , %eps );
ssize = optimsimplex_size ( s1 , "sigmaminus" );
assert_close ( ssize , 1.0 , %eps );
s1 = optimsimplex_destroy(s1);

// Test unknown size method
s1 = optimsimplex_new();
cmd = "optimsimplex_size ( s1 , ''foo'' )";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimsimplex_size: Unknown simplex size method foo";
assert_equal ( computed , expected );
s1 = optimsimplex_destroy(s1);

