// Non-regression test file for bug 1784
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 23 janvier 2006

mode(-1);
clear
A=hypermat([3,3,2]);
try
    A(1,1:3,1:3)=[1 2 3;4 5 6;7 8 9];
    ok=%t
catch
    ok=%f
end
if ok then
    ok=and(A==hypermat([3,3,3],[1;0;0;4;0;0;7;0;0;2;0;0;5;0;0;8;0;0;3;0;0; ...
    6;0;0;9;0;0]))
end
affich_result(ok,1784);
clear
