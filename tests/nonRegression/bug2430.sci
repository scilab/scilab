// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : June 2007

mode(-1) ;
clear ;
BugNumber=2430
b=sparse(1:16);
T=and(abs(full(2\b)-2\full(b))<100*%eps);
affich_result(T,BugNumber) ;
clear ;
