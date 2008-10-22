// Non-regression test file for bug 1956
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 4 mai 2006

mode(-1);
clear a d
a.b=[1 2 3];
d(1)=a
r=and(d==a)
d(2)=a;
r=r&and(d(1)==a)
r=r&and(d(2)==a)

affich_result(r,1956);
clear
