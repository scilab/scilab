// <-- Non-regression test for bug 2414 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2414
//
// <-- Short Description -->
//    assignment of empty string an empty variable has disastrous results. Probably a
//    parser bug, for statements of the form a=string(a), with a=[]

// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 05 mai 2007

a=[];
a = string(a);

if( (type(a) == 1) & (typeof(a) == "constant") & and(size(a) == [0 0]) & (a($,1)==[]) ) then
	affich_result(%T,2414);
else
	affich_result(%F,2414);
end
