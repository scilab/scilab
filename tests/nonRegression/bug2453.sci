// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : Aug 2007

mode(-1) ;
clear ;
BugNumber=2453;
fd=mopen('bug2453.dat','r');
Lx=mfscanf(-1,fd,'%g %g\n');mclose(fd); 
T=size(Lx,1)==40000;
affich_result(T,BugNumber) ;
clear ;

