// <-- Non-regression test for bug 1084 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1084
//
// <-- Short Description -->
//    Problem with resetting seed for rand 'normal' 
//    distribution.  As you can see from the attached code, I'm 
//    resetting the seed and generating a rand 'normal' 
//    distribution, however I only get the same results every 
//    other time.  This should produce the same results every 
//    time.  Note that the 'uniform' distribution does generate 
//    the same results every time.


// Non-regression test file for bug 1084
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

rand('seed',0);
A = rand(1,3,'normal');
rand('seed',0);
B = rand(1,3,'normal');
rand('seed',0);
rand('seed',0);
C = rand(1,3,'normal');

if and(A==B) & and(B==C) & and(A==C) then
	affich_result(%T,1084);
else
	affich_result(%F,1084);
end
