// <-- Non-regression test for bug 1684 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1684
//
// <-- Short Description -->
//    I can't compute the following expression:
//    A = 1 + -2;
//    I need to enter A = 1 + (-2);
//    I've got a program which generates automaticaly matlab scripts with this kind of
//    expressions and I don't want to change it to test is the value is negative and
//    then add parenthesis.

// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 6 mars 2006

a=1+-1;
ok1=a==0;
a=1-+1;
ok2=a==0;
a=1++1;
ok3=a==2;
a=1--1;
ok4=a==2;
a=1*-1;
ok5=a==-1;
a=1*+1;
ok6=a==1;
a=2*+++--2;
ok7=a==4;

a=3+-1*+++--2;
ok8=a==1;

affich_result(ok1&ok2&ok3&ok4&ok5&ok6&ok7&ok8,1684);
clear
