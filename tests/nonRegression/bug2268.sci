// Non-regression test file for bug 2266
// Serge Steer - Scilab Project
// Copyright INRIA
// 2 janv 2007

mode(-1);
clear;


free=getmemory() ;
for k=1:10000
  foo=msscanf('1.234 44 55 66 77 88 88 ', '%f %f %f %f %f %f %f '); 
end
leak=free-getmemory()
T=leak<=4;
affich_result(T,2266);
