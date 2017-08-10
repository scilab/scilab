// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5359 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5359
//
// <-- Short Description -->
// When an atoms module cannot be loaded at startup, fileseps in the path
// displayed in the error message are eaten.

load("SCI/modules/atoms/macros/atoms_internals/lib");

atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");
atomsSystemUpdate();
atomsInstall("toolbox_1");
if ~atomsIsInstalled("toolbox_1") then pause, end

// Now rename the install directory of toolbox_1
// Restart scilab
// Have a look to the error


// Some cleaning
atomsRemove("toolbox_1");
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
atomsSystemUpdate();
