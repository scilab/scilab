// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 13758 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13758
//
// <-- Short Description -->
// x_mdialog interpreted 'F' and 'T' as boolean values
// whereas it should interpret '%F' and '%T' instead.

xcos("SCI/modules/xcos/tests/nonreg_tests/bug_13758.zcos");

// Double-click on SampleCLK block (top left block)
// In the parameters dialog, check that "Sample time" is a text field containing T and not a checkbox.
