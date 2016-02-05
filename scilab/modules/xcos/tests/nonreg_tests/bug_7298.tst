// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7298 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7298
//
// <-- Short Description -->
// Creating a superblock diagram using the region to superblock action
// throws an exception on updating the exported ports.

xcos();
// put 2 SUM_f blocks
// link them
// select one block
// Right-click -> region to superblock

