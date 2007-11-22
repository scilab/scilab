// <-- Non-regression test for bug 967 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=967
//
// <-- Short Description -->
//    please read the messages posted to comp.soft-
//    sys.math.scilab with thread subject 'Nan(not-a-number), Inf
//    (infinity) question...' (1 sep 2004) and
//    'Runtime error...' (31 aug 2004)


// Non-regression test file for bug 967
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

m=[5 %inf;2 %i];

if ~or(m==%nan) then
	affich_result(%T,967);
else
	affich_result(%F,967);
end
