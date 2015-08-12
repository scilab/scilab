// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2098 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2098
//
// <-- Short Description -->
//    showprofile(test) makes Scilab crash if the function 'test' has not 
//    been prepared for profiling, as demonstrated by the following example:
//
//    -->function test
//    -->disp('test')
//    -->endfunction
//
//    -->test
//
//     test
//
//    -->showprofile(test)  // this line makes Scilab exit silently

function test
	disp('test');
endfunction

ierr = execstr('showprofile(test)','errcatch');
if ierr <> 10000 then pause,end
