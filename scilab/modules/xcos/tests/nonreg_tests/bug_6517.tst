// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6517 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6517
//
// <-- Short Description -->
// This test validates the (Xcos file loading, export to hdf5)(Java part) and
// (import from hdf5)(Scilab part)

result = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_6517.zcos");
if (result <> %T) then pause,end
if (isdef("scs_m") <> %T) then pause,end

