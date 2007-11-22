// <-- Non-regression test for bug 2264 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2264
//
// <-- Short Description -->
//    lcm gives wrong value for 3-element vectors(cf bug 666)


// Non-regression test file for bug 2264
// Serge Steer - Scilab Project
// Copyright INRIA
// 2 janv 2007

mode(-1);
clear;

[q,fact]=lcm(int32([-6 5 7 12 4])); 
T=q==int32(420);
[q,fact]=lcm(int32([6 5 7 3 4])) ;
T=T&(q==int32(420));
[q,fact]=lcm(int32([-6 5 1 3 4]));
T=T&(q==int32(60));
[q,fact]=lcm(int32([-6 5 0 3 4]));
T=T&(q==int32(0));

affich_result(T,2264);
