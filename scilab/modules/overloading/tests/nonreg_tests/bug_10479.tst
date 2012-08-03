// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10479 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10479
//
// <-- Short Description -->
//The max and min functions where incorrect for N-D array

A=rand(3,3,3);
M=max(A,0.5);
assert_checkequal(M,matrix(max(A(:),0.5),size(A)));
M=max(0.5,A);
assert_checkequal(M,matrix(max(A(:),0.5),size(A)));


B=rand(3,3,3);
M=max(A,B);
assert_checkequal(M,matrix(max(A(:),B(:)),size(A)));

A=matrix(1:27,3,3,3);
assert_checkequal(max(A,'r'),matrix(3:3:27,1,3,3));
assert_checkequal(max(A,'c'),matrix([7:9 16:18 25:27],3,1,3));
