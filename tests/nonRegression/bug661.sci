// Non-regression test file for bug 661
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 10 janvier 2006

mode(-1);
clear
ok=and(abs(quart(1:3)-[1.25;2;2.75])<10*%eps)
affich_result(ok,661);
