// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1591 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1591
//
// <-- Short Description -->
//    Function TCL_GetVar has strange behaviour with matrixes
//    known that TCL Array are in fact Hashtables.

TCL_EvalStr("array set ta {}");
TCL_EvalStr("set ta(1,1) one");
TCL_EvalStr("set ta(1,2) two");
TCL_EvalStr("set ta(2,1) three");

// ta is a 2x2 Matrix
if and(size(TCL_GetVar("ta")) == [2 2]) == %F then pause,end


// now ta become an hashtabl
TCL_EvalStr("set ta(deux,deux) four");

if and(size(TCL_GetVar("ta")) == [4 1]) == %F then pause,end


a = [1 2 3;4 5 6; 7 8 9]

TCL_SetVar("tcl_a", a)

// a is a 3x3 matrix
if and(size(TCL_GetVar("tcl_a")) == [3 3]) == %F then pause,end


// adding a new value change a into a 50x10 matrix
TCL_SetVar("tcl_a(50,10)", "50");

if and(size(TCL_GetVar("tcl_a")) == [50 10]) == %F then pause,end


