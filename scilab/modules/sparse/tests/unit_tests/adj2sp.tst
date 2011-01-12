// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
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
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

//
// Check the adjacency representation of a simple matrix
xadj = [1 3 4 7 9 11]';
iadj = [3 5 3 1 2 4 3 5 1 4]';
v = [1 2 3 4 5 6 7 8 9 10]';
B=adj2sp(xadj,iadj,v);
A = [
0 0 4 0 9
0 0 5 0 0
1 3 0 7 0
0 0 6 0 10
2 0 0 8 0
];
C=sparse(A);
assert_equal ( and(B==C) , %t );
//
// Check the adjacency representation of a simple matrix
xadj = [1 2 3 4 5 5 6 6 7 8 9]';
iadj = [2 5 2 3 1 2 7 6]';
v = [3 7 5 3 6 5 2 3]';
C=adj2sp(xadj,iadj,v);
A = [
0 0 0 0 0 6 0 0 0 0
3 0 5 0 0 0 0 5 0 0
0 0 0 3 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 7 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 3
0 0 0 0 0 0 0 0 2 0
];
B=sparse(A);
assert_equal ( and(B==C) , %t );
//
// Test the C=adj2sp(xadj,iadj,v,mn) calling sequence
xadj = [1 2 3 4 5 5 6 6 7 8 9]';
iadj = [2 5 2 3 1 2 7 6]';
v = [3 7 5 3 6 5 2 3]';
mn=[7 10];
C=adj2sp(xadj,iadj,v,mn);
A = [
0 0 0 0 0 6 0 0 0 0
3 0 5 0 0 0 0 5 0 0
0 0 0 3 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 7 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 3
0 0 0 0 0 0 0 0 2 0
];
B=sparse(A);
assert_equal ( and(B==C) , %t );

