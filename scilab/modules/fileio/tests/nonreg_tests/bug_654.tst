// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=654
//
// <-- Short Description -->
//    listfiles(getenv('SCI')+"/tcl") gives wrong result. 
//
//    - or the syntax is accepted and the result should be correct
//    - or an error message should be displayed
//
//    of course listfiles(getenv('SCI')+"/tcl/") is OK

if or( listfiles(getenv('SCI')+"/modules/tclsci") <> listfiles(getenv('SCI')+"/modules/tclsci/") ) then pause,end
