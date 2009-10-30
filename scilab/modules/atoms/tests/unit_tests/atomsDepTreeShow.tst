// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");



// Do not use the autoload system
config_autoload = atomsGetConfig("autoloadAddAfterInstall");
config_Verbose  = atomsGetConfig("Verbose");
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsSystemUpdate();

// Install toolbox 5
atomsInstall("toolbox_5");

// Show the dependency tree
atomsDepTreeShow("toolbox_5");

// Load the 2nd scenario : See scene2.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsSystemUpdate();

// Update modules
atomsUpdate();

// Show the dependency tree
atomsDepTreeShow("toolbox_5");

// Remove toolbox 5
atomsRemove("toolbox_5");

// Restore original values
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl("http://atoms.scilab.org");
