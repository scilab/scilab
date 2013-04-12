// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 9024 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9024
//
// <-- Short Description -->
// Opening a non-existing file lead to problems when closing Xcos.
//

execstr("xcos(); xcos(""DoesNotExist.zcos"")");
// Answer NO to the create file dialog
// Close the Xcos window
// check that the palette is also closed


