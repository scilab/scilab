// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
// <-- Non-regression test for bug NotReportedYet -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8556

xcos();
// put 2 SUM_f block
// Link SUM_f#1 output port to SUM_f#1 input port
//    Check the right creation of the link#1
// Link the link#1 to the SUM_f#2 input port
//    Check that a split block has been added
//    Check that a link has been created

