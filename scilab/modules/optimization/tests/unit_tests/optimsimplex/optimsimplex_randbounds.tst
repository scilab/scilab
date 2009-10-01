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
// Test randbounds with default number of vertices
//
s1 = optimsimplex_new ( "randbounds" , [-1.2 1.0], rosenbrock, ...
  [-5.0 -5.0] , [5.0 5.0] );
computed = optimsimplex_getall ( s1 );
assert_equal ( size(computed,1) , 3 );
assert_equal ( size(computed,2) , 3 );
expected = [
  %T %T  
  %T %T  
  %T %T  
];
assert_equal ( computed(1:3,2:3) < 5.0 , expected );
assert_equal ( computed(1:3,2:3) > -5.0 , expected );
s1 = optimsimplex_destroy ( s1 );

//
// Test randbounds with 5 vertices
//
s1 = optimsimplex_new ( "randbounds" , [-1.2 1.0], rosenbrock, ...
  [-5.0 -5.0] , [5.0 5.0], 5 );
computed = optimsimplex_getall ( s1 );
assert_equal ( size(computed,1) , 5 );
assert_equal ( size(computed,2) , 3 );
expected = [
  %T %T  
  %T %T  
  %T %T  
  %T %T  
  %T %T  
];
assert_equal ( computed(1:5,2:3) < 5.0 , expected );
assert_equal ( computed(1:5,2:3) > -5.0 , expected );
s1 = optimsimplex_destroy ( s1 );

//
// Test optimsimplex_randbounds
//
function [ y , myobj ] = mycostf ( x , myobj )
  y = rosenbrock(x);
  myobj.nb = myobj.nb + 1
endfunction

//
// Test randbounds with additionnal object
//
mydude = tlist(["T_MYSTUFF","nb"]);
mydude.nb = 0;
s1 = optimsimplex_new ();
[ s1 , mydude ] = optimsimplex_new ( "randbounds" , [-1.2 1.0], mycostf, ...
  [-5.0 -5.0] , [5.0 5.0], 5 , mydude );
computed = optimsimplex_getall ( s1 );
assert_equal ( size(computed,1) , 5 );
assert_equal ( size(computed,2) , 3 );
assert_equal ( mydude.nb , 5 );
expected = [
  %T %T  
  %T %T  
  %T %T  
  %T %T  
  %T %T  
];
assert_equal ( computed(1:5,2:3) < 5.0 , expected );
assert_equal ( computed(1:5,2:3) > -5.0 , expected );
s1 = optimsimplex_destroy ( s1 );

