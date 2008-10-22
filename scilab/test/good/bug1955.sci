// Non-regression test file for bug 1955
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 3 mai 2006

mode(-1);
clear

global ghm

ghm=matrix(1:8,2,4);
ghm=matrix(ghm,2,2,2);

r= and(ghm==matrix(1:8,2,2,2))
affich_result(r,1955);
clear
