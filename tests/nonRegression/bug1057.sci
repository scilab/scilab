// <-- Non-regression test for bug 1057 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1057
//
// <-- Short Description -->
//    Crash when entering the following in the command window:
//
//    1.7977E+308
//
//    Actually, every number greater than 1.797E+308 crashes 
//    scilab (scilab3.0 under linux gives the correct answer Inf).
//
//    A popup window appears with the following message:
//
//    forrtl: severe(64): input conversion error, unit -5, file 
//    Internal List Directed Read
//    Image [...]
//    LibScilab.dll [...]
//    [...]
//
//    [...] is text I don't think is important.
//
//    Basically overflow is translated in crashes where it should 
//    be Inf.


// Non-regression test file for bug 1057
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

mode(-1);
clear;

if 1.7977E+308 == %inf  then
	affich_result(%T,1057);
else
	affich_result(%F,1057);
end
