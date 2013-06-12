// Non-regression test file for bug 205
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 09 june 2006

mode(-1);
clear;

N=30;
A=rand(N,N);
s=poly(0,"s");
P=determ( eye(N,N)*%s - A );

if degree(P)==30  then
    affich_result(%T,205);
else
    affich_result(%F,205);
end

clear
