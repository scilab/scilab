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

// With base simplex and no function
simplex = [...
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
s2 = optimsimplex_new ( "oriented" , s1 );
computed = optimsimplex_getall ( s2 );
expected = [...
24.2 -1.2 1.0
0.0 -1.7 1.0
0.0 -1.2 0.5
];
assert_close ( computed , expected , %eps );
s1 = optimsimplex_destroy(s1);
s2 = optimsimplex_destroy(s2);

// With basic function
simplex = [...
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
s2 = optimsimplex_new ( "oriented" , s1 , rosenbrock );
computed = optimsimplex_getall ( s2 );
expected = [...
24.2 -1.2 1.0
364.5 -1.7 1.0
93.2 -1.2 0.5
];
assert_close ( computed , expected , %eps );
s1 = optimsimplex_destroy(s1);
s2 = optimsimplex_destroy(s2);
//
// Test with an additional argument
//
myobj = tlist(["T_MYSTUFF","nb"]);
myobj.nb = 0;
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction
simplex = [...
24.2 -1.2 1.0
93.6 -0.2 1.0
36.2 -1.2 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
[s2,myobj] = optimsimplex_new ( "oriented" , s1 , mycostf , myobj );
computed = optimsimplex_getall ( s2 );
expected = [...
24.2 -1.2 1.0
364.5 -1.7 1.0
93.2 -1.2 0.5
];
assert_close ( computed , expected , %eps );
assert_equal ( myobj.nb , 2 );
s1 = optimsimplex_destroy(s1);
s2 = optimsimplex_destroy(s2);

