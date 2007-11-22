// <-- Non-regression test for bug 2410 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2410
//
// <-- Short Description -->
//    string(boolean array) is incredibly slow

// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

tic();
string(zeros(1,10000)==1);
a = toc();

if( a < 0.1 ) then
	affich_result(%T,2410);
else
	affich_result(%F,2410);
end
