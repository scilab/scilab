// Non-regression test file for bug 2263
// Serge Steer - Scilab Project
// Copyright INRIA
// 2 janv 2007

mode(-1);
clear;
function [a,b,c,d,e]=test()
  a=1;b=2;c=3;d=4;e=5;
endfunction
[out.a.t1,out.a.t2,out.a.t3,out.a.t4]=test();

T=and(out==mlist(["st","dims","a"],int32([1,1]),..
mlist(["st","dims","t4","t3","t2","t1"],int32([1,1]),4,3,2,1))) 

affich_result(T,2263);
