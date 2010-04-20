// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH XCOS -->
//
// <-- Non-regression test for bug 6776 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6776
//
// <-- Short Description -->
// The export_to_hdf5 return status was not used when loading a scicos
// diagram.

xcos(SCI + "/modules/xcos/tests/nonreg_tests/Antrieb3.cos");
// Check that the diagram hasn't been loaded and the error dialog has been poped
// up
