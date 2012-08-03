// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 10366 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10366
//
// <-- Short Description -->
// a warning about fun2string redefinition was displayed when we used 
// "Edit label generator" and click "Ok"
//

plot2d();
// button datatips
// right click on a curve
// item "Open curve tip style editor"
// button "Edit label generator"
// button OK

// check in console that you have no warning output
