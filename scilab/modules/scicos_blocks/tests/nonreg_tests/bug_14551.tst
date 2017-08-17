// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14551 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14551
//
// <-- Short Description -->
// In the icon of CLR and DLR blocks, multichar exponents were badly rendered

xcos("SCI/modules/scicos_blocks/tests/nonreg_tests/bug_14551.zcos");

// In the icon of the block, all exponents must be correctly displayed:
// 1 + 3*s^10 / 1 - 3*s^2 - 4.3*s^0.55 + s^12.3

// * Double-click on the icon
// * Change existing exponents with multichar values
// * Press "OK": => the updated rendering must be still OK

