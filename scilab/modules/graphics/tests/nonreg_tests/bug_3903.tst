// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3903 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3903
//
// <-- Short Description -->
// "xdel" of a non existent window crashes Scilab
// 

// close all figures
xdel(winsid());


// try to delete a non existent window
execstr("xdel(-1)", "errcatch");

// create a figure
f = scf(123456);
// destroy it
delete(f);
// try again
execstr("xdel(123456)", "errcatch");

scf(1);
scf(10);
// figure with id 9 does not exists
execstr("xdel([1, 10, 9])", "errcatch");

// check that last call to xdel was canceled and did not destroy any window.
if (winsid() <> [1, 10]) then pause; end
