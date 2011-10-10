// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 9804 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9804
//
// <-- Short Description -->
// Starting Scilab with a external module emptied the demo list menu.


// build toolbox_skeleton module in SCI/contrib/toolbox_skeleton
// exec builder.sce

// load this module
// exec loader.sce

// Launch Scilab Demonstration GUI: Menu ?/Scilab Démonstrations

// check that demos list menu is not empty

// restart scilab and do not load toolbox_skeleton and check also that demos list menu is not empty

// install a ATOMS modules and check also that demos list menu is not empty.