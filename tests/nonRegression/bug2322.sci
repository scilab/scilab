// <-- Non-regression test for bug 2322 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2322
//
// <-- Short Description -->
//    emptystr(0,1) returns an empty matrix of size 0 x 1 which is not coherent with
//    current empty matrix managment.

// Serge Steer - Scilab Project
// Copyright INRIA
// 19/02/2007

r=%t
r=r&and(size(emptystr(0,1))==[0 0]);
r=r&and(size(emptystr(1,0))==[0 0]);
r=r&and(size(emptystr(0,3))==[0 0]);
r=r&and(size(emptystr(5,0))==[0 0]);
r=r&and(size(emptystr(0,0))==[0 0]);
r=r&and(size(emptystr(3,5))==[3 5]);

affich_result(r,2322);
