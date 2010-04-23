// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6784 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6784
//
// <-- Short Description -->
// When a copy of Scilab is installed with its ATOMS modules just by
// copying/pasting a full original installation directory to another place, the
// ATOMS modules of the cloned installation can no longer be launched at the
// session startup.

atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");
atomsInstall("toolbox_1");

// Move the scilab installation to another path
// Restart scilab
// If toolbox_1 is successfully loaded, the bug is fixed
