// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5075 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5075
//
// <-- Short Description -->
//    This scilab script is considerer as a binary file by the Scilab text editor.
//    So, it is opened read only and it's not possible to modify it.

editor('SCI/modules/core/macros/add_demo.bin')
// try with any other type of file







