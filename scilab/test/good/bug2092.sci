// Non-regression test file for bug 2092
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 17 aout 2006

mode(-1);
clear;

A = mad([1:10;2:11],2);
B = mad([1:10;2:11],"c");

if and(A == B) then
    affich_result(%T,2092);
else
    affich_result(%F,2092);
end

clear
