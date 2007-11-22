// <-- Non-regression test for bug 2430 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2430
//
// <-- Short Description -->
//    Wrong results when solving linear systems when the matrix
//    is sparse and 1x1, and rhs is compatible and sparse. 
//    In the example below, M\b and full(M)\b give a wrong result,
//    whereas M\full(b) and full(M)\full(b) are ok. 
//
//    Strangely, when the size of b is less than 10, then all
//    cases give the correct answer !


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
