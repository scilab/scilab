// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// We need a clean version
// =============================================================================
if ~isempty( atomsGetInstalled() ) then pause, end

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

//force official ATOMS repository
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));

// Set some parameters for the test
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// 1st test-case : Just install the toolbox 5
// =============================================================================
atomsInstall("toolbox_5V6");

// Check if the module is really installed

if ~ and( atomsIsInstalled( ["toolbox_5V6" "1.2" ; ..
    "toolbox_4V6" "1.1" ; ..
    "toolbox_2V6" "1.0" ; ..
"toolbox_1V6" "2.0"])) then pause, end

atomsLoad("toolbox_5V6");

if ~ atomsIsLoaded("toolbox_5V6") then pause, end
if ~ atomsIsLoaded(["toolbox_5V6" "1.2"]) then pause, end

if ~ and(atomsIsLoaded(["toolbox_5V6"; ..
    "toolbox_2V6"; ..
    "toolbox_1V6"; ..
"toolbox_4V6"])) then pause, end

if ~ and(atomsIsLoaded(["toolbox_5V6" "1.2"; ..
    "toolbox_2V6" "1.0"; ..
    "toolbox_1V6" "2.0"; ..
"toolbox_4V6" "1.1"])) then pause, end

if or( t5_version() <> ["Toolbox 5 -> version = 1.2"; ..
    "Toolbox 4 -> version = 1.1"; ..
    "Toolbox 2 -> version = 1.0"; ..
"Toolbox 1 -> version = 2.0" ] ) then pause, end

atomsRemove("toolbox_5V6");

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);
