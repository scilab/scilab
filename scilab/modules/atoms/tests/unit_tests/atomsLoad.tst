// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// We need a clean version
// =============================================================================
if ~isempty( atomsGetInstalled() ) then pause, end 

// Set some parameters for the test
// =============================================================================
config_autoload = atomsGetConfig("autoloadAddAfterInstall");
config_Verbose  = atomsGetConfig("Verbose");
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// 1st test-case : Just install the toolbox 5
// =============================================================================

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");

atomsInstall("toolbox_5");

// Check if the module is really installed

if ~ and( atomsIsInstalled( ["toolbox_5" "1.0" ; ..
                             "toolbox_4" "1.0" ; ..
                             "toolbox_2" "1.0" ; ..
                             "toolbox_1" "1.0"])) then pause, end

atomsLoad("toolbox_5");

if ~ atomsIsLoaded("toolbox_5") then pause, end
if ~ atomsIsLoaded(["toolbox_5" "1.0"]) then pause, end

if ~ and(atomsIsLoaded(["toolbox_5"; ..
                        "toolbox_2"; ..
                        "toolbox_1"; ..
                        "toolbox_4"])) then pause, end

if ~ and(atomsIsLoaded(["toolbox_5" "1.0"; ..
                        "toolbox_2" "1.0"; ..
                        "toolbox_1" "1.0"; ..
                        "toolbox_4" "1.0"])) then pause, end

if or( t5_version() <> ["Toolbox 5 -> version = 1.0"; ..
                        "Toolbox 4 -> version = 1.0"; ..
                        "Toolbox 2 -> version = 1.0"; ..
                        "Toolbox 1 -> version = 1.0" ] ) then pause, end

atomsRemove("toolbox_5");

// Restore original values
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
