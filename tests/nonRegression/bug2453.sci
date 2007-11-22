// <-- Non-regression test for bug 2453 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2453
//
// <-- Short Description -->
//    mfscanf and probably the other variants msscanf mscanf may crash Scilab when
//    first argument is -1 (any number of lines)


// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : Aug 2007

BugNumber=2453;
fd=mopen('bug2453.dat','r');
Lx=mfscanf(-1,fd,'%g %g\n');mclose(fd); 
T=size(Lx,1)==40000;
affich_result(T,BugNumber) ;
