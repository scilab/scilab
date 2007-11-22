// <-- Non-regression test for bug 1183 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1183
//
// <-- Short Description -->
//    @?

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

A =1@1
B=lasterror()

if B=='Missing operator, comma, or semicolon' then
	affich_result(%T,1183);
else
	affich_result(%F,1183);
end
