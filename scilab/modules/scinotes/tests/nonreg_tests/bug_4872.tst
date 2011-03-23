// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 4872 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4872
//
// <-- Short Description -->
//    editor(<binary file>) opens a blank file without any warning.


editor('SCI/modules/core/macros/add_demo.bin')

// A warning message is now added when a binary file is opened

// For now we have decided to do not show a warning message
// due to binary file detection problems




