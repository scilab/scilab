// Non-regression test file for bug 1802
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 30 janvier 2006

mode(-1);
clear

a=rand(100,200);
timer();sci2exp(a);t=timer();
affich_result(t<3,1802);
clear
