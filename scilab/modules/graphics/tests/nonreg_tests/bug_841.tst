// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 841 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=841
//
// <-- Short Description -->

// Ordinate of a plot is not properly calculated,
// so that the numbers on the left axis are unequally spaced.
// This is best understood by an example (see below "Steps to
// reproduce bug")

// x = [0:0.1:5]';
// y = cos(x);
// plot2d(x,y); // BUG: ordinate show -0.9, -0.6, -0.4, etc...
//              //      So the first delta is 0.3 and then 0.2.
// plot2d([0,5],[0,0]); // By plotting the zero line, we clearly
//                      // see it doesn't go through zero.		

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 29 novembre 2007

 // Non reg test on string format in 2D Axes

a              = gca();
a.tight_limits = 'on';
x              = [0:0.1:5]';
y              = cos(x);
plot2d(x,y);          // 3 numbers after comma // CHECK HERE
plot2d([0,5],[0,0]);  // the zero is well placed  // CHECK HERE

// pretty display test:
a.tight_limits="off"; // CHECK HERE
