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
