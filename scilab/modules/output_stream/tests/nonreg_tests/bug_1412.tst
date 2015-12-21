// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1412 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1412
//
// Causes scilab to hang, using 95% of CPU.
// and display a incorrect error

A(3,3)=%s;
ierr = execstr("A(:)=%z;","errcatch");
assert_checktrue(ierr <> 0);
