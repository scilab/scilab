// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6596 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6596
//
// <-- Short Description -->
// When the ATOMS GUI opens, the list of already installed modules could be
// displayed, instead of the message "No module selected". This list could also
// be displayed by clicking on a special keywork, e.g. the title of the left
// column. Another way for showing installed modules could be to star their name
// in the left list. Both ways are not exclusive

load("SCI/modules/atoms/macros/atoms_internals/lib");

atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");
atomsSystemUpdate();
if ~atomsIsInstalled("toolbox_1") then pause, end

atomsGui();

// The list of installed modules should be starred on the left and appears on the
// right

atomsRemove("toolbox_1");

// Some cleaning
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
atomsSystemUpdate();
