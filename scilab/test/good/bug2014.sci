// Non-regression test file for bug 2014
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : june  2006

mode(-1);
clear
l1=list(1,2,3);l2=list(1,2);l3=list();
r=isequal(l1,l1)&isequal(l3,l3)&~isequal(l1,l2)&~isequal(l3,l1)

affich_result(r,2014);
clear
