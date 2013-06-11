// Non-regression test file for bug 1991
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 15 mai 2006

mode(-1);
clear
[a,b]=(3,int32(5));
r=a==3&b==int32(5);
a=(3 + int32(5));
r=r&(a==int32(8));

[a,b,c]=(3,(2),sin(5));
r=r&a==3&b==2&c==sin(5);

affich_result(r,1991);
clear
