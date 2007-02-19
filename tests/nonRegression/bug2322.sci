// Non-regression test file for bug 2322
// Serge Steer - Scilab Project
// Copyright INRIA
// 19/02/2007

mode(-1);
clear;
r=%t
r=r&and(size(emptystr(0,1))==[0 0]);
r=r&and(size(emptystr(1,0))==[0 0]);
r=r&and(size(emptystr(0,3))==[0 0]);
r=r&and(size(emptystr(5,0))==[0 0]);
r=r&and(size(emptystr(0,0))==[0 0]);
r=r&and(size(emptystr(3,5))==[3 5]);

affich_result(r,2322);
clear
