// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8110 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8110
//
// <-- Short Description -->
// When removing a module, its main directory is not deleted even if it is empty.

atomsInstall("number","allusers");
atomsRemove("number","allusers");

// The directory SCI/contrib/number should not exist
