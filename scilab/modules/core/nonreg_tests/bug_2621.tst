// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2621 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2621
//
// <-- Short Description -->
//
// errcatch(n,'continue') with an error greater than 9999 doesn't catch the error
//

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 29 novembre 2007

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
