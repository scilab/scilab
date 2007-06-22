// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : June 2007

mode(-1) ;
clear ;
BugNumber=2421
T=execstr('[a(:), v(1)] = (1, 2)','errcatch')==0
affich_result(T,BugNumber) ;
clear ;
