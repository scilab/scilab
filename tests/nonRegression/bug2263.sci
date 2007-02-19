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
T=%t;
T=T&and(out==mlist(["st","dims","a"],int32([1,1]),..
		   mlist(["st","dims","t4","t3","t2","t1"],int32([1,1]),4,3,2,1))) ;

clear out
[out.a.t1,t2,out.a.t3,t4]=test();
T=T&and(out==mlist(["st","dims","a"],int32([1,1]),..
		   mlist(["st","dims","t3","t1"],int32([1,1]),3,1)))&t1==1&t4==4;

clear a
[a.t1,a.t2,a.t3,a.t4]=test();
T=T& and(a==mlist(["st","dims","t4","t3","t2","t1"],int32([1,1]),4,3,2,1));

clear a
[t1,a.t2,t3,a.t4]=test();
T=T& and(a==mlist(["st","dims","t4","t2"],int32([1,1]),4,2))&t1==1&t3==3;

[t1,t2,t3,t4]=test();
T=T&and([t1 t2 t3 t4]==[1 2 3 4]);

affich_result(T,2263);
