// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");

// Do not use the autoload system
config_autoload = atomsGetConfig("autoloadAddAfterInstall");
config_Verbose  = atomsGetConfig("Verbose");
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Install the toolbox 5
// =============================================================================

atomsInstall("toolbox_3");
atomsShow("toolbox_3");
atomsRemove("toolbox_3");

// Restore original values
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl("http://atoms.scilab.org");
