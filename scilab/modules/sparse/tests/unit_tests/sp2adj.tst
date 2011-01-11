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
A = [
0 0 4 0 9
0 0 5 0 0
1 3 0 7 0
0 0 6 0 10
2 0 0 8 0
];
B=sparse(A);
mn=size(B);
[xadj,iadj,v]=sp2adj(B);
m = max(iadj);
n = size(xadj,"*")-1;
assert_equal ( mn(1) , m );
assert_equal ( mn(2) , n );
expected_xadj = [1 3 4 7 9 11]';
expected_iadj = [3 5 3 1 2 4 3 5 1 4]';
expected_v = [1 2 3 4 5 6 7 8 9 10]';
assert_equal ( and(expected_xadj == xadj) , %t );
assert_equal ( and(expected_iadj == iadj) , %t );
assert_equal ( and(expected_v == v) , %t );
// j is the column index
for j = 1 : size(xadj,"*")-1
  irows = iadj(xadj(j):xadj(j+1)-1);
  vcolj = v(xadj(j):xadj(j+1)-1);
end
//
// Check the adjacency representation of a simple matrix
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
mn=size(B);
[xadj,iadj,v]=sp2adj(B);
m = max(iadj);
n = size(xadj,"*")-1;
assert_equal ( mn(1) , m );
assert_equal ( mn(2) , n );
expected_xadj = [1 2 3 4 5 5 6 6 7 8 9]';
expected_iadj = [2 5 2 3 1 2 7 6]';
expected_v = [3 7 5 3 6 5 2 3]';
assert_equal ( and(expected_xadj == xadj) , %t );
assert_equal ( and(expected_iadj == iadj) , %t );
assert_equal ( and(expected_v == v) , %t );
//
// Check that sp2adj matches adj2sp
A = sprand(100,50,.05);
[xadj,iadj,v]= sp2adj(A);
[n,m]=size(A);
p = adj2sp(xadj,iadj,v,[n,m]);
assert_equal ( and(A-p==0) , %t );

