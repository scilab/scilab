// <-- Non-regression test for bug 1956 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1956
//
// <-- Short Description -->
//    this works for example:
//    a=10; // a constant
//    for i=1:2
//     d(i)=a;
//    end
//    but it doesn't work if 'a' is a structure.
//    I find a solution for a structure:
//    a.b=[1 2 3];
//    a.c=[4 5 6]
//    but it's not smart and clear code...


// Non-regression test file for bug 1956
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2006
// Date : 4 mai 2006

mode(-1);
clear a d
a.b=[1 2 3];
d(1)=a
r=and(d==a)
d(2)=a;
r=r&and(d(1)==a)
r=r&and(d(2)==a)

affich_result(r,1956);
clear
