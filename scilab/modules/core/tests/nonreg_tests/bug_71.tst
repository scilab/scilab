// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 71 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=71
//
// <-- Short Description -->
//    The macr2lst() function crashes when a macro including 
//    an "exit" call is given as parameter


function result = bug()
	// This part replay bug
	deff("foo","exit")
	result=execstr("macr2lst(foo)","errcatch","n")
	clear foo;
endfunction

if bug() <> 0 then pause,end
