// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 573 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=573
//
// <-- Short Description -->
//    On Win2000 the functions tk_getdir() and tk_getfile() do 
//    not accept directory names that contain space characters. 
//    Given the existing path:
//      "C:/has space"
//
//    tk_getdir("C:/has space") generates an error.
//    tk_getfile("*","C:/has space") gnerates an error.
//    The error appears to be in TK rather than Scilab itself.
//
//    A partial work-around is:
//    chdir("C:/has space")
//    tk_getdir()
//
//    The function xgetfile() does NOT have this problem. 
//    Unfortunately there is no xgetdir() function!

if MSDOS then
	disp("windows only, selection a directory and click OK");
	tk_getdir("c:/has space")
	tk_getfile('*','C:/has space');
end
