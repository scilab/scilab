// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->

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
  if ( and ( computed == expected ) ) then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

x = factorial ( 1 );
assert_equal ( x , 1 );
x = factorial ( 2 );
assert_equal ( x , 2 );
x = factorial ( [1 2 3 4] );
assert_equal ( x , [1 2 6 24] );
x = factorial ( 170 );
assert_close ( x , 7.25741561530799896739e306 , 10 * %eps );
// Test with a matrix
n = [
1 2 3
4 5 6
7 8 9
];
x = factorial ( n );
e = [
    1.       2.        6.       
    24.      120.      720.     
    5040.    40320.    362880.  
];
assert_equal ( x , e );
// Test with an hypermatrix
clear n;
clear e;
n(1,1,1,1:2)=[1 2];
x = factorial ( n );
e(1,1,1,1:2)=[1 2];
assert_equal ( x , e );
clear n;
clear e;

