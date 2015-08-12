// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2621 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2621
//
// <-- Short Description -->
//
// errcatch(n,'continue') with an error greater than 9999 doesn't catch the error
//

function bug_errcatch_one
	errcatch(9999,"continue")
	error(9999)
	disp("TEST PASSED")
endfunction

function bug_errcatch_two
	errcatch(10000,"continue")
	error(10000)
	disp("TEST PASSED")
endfunction

bug_errcatch_one()
bug_errcatch_two()
