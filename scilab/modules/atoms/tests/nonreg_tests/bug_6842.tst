// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6842 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6842
//
// <-- Short Description -->
// When an ATOMS module has several versions and that, e.g. the oldest is
// installed by using atomsInstall on Scilab's command line, atomsGUI claims
// that the newest version is installed.

load("SCI/modules/atoms/macros/atoms_internals/lib");

atomsRepositorySetOfl("http://scene11.test.atoms.scilab.org");
atomsSystemUpdate();
atomsInstall(["toolbox_4","1.0-1"]);
if ~atomsIsInstalled("toolbox_4") then pause, end
atomsGui();

// Check if there is a warning when the description of toolbox_1 is displayed.

atomsRemove("toolbox_1");
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
