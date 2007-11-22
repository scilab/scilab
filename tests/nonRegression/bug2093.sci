// <-- Non-regression test for bug 2093 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2093
//
// <-- Short Description -->
//    matrix(A,q) doesn't return a q*1 matrix when A is a 1*1*q matrix and q a scalar.
//    Instead, it returns a mlist.


// Non-regression test file for bug 2093
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 28 août 2006

mode(-1);
clear;

A = rand(1,1,6);
B = matrix(A,6);
C = matrix(A,[6,1]);

if (type(B) == 1) & (type(C) == 1) then
	affich_result(%T,2093);
else
	affich_result(%F,2093);
end

clear
