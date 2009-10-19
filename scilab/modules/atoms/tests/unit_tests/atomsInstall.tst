// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");

// Do not use the autoload system
config_autoload = atomsGetConfig("autoload");
atomsSetConfig("autoload","False");

// Install the toolbox 5
atomsInstall("toolbox_5");

// Remove the toolbox 5
atomsRemove("toolbox_5");

// Restore original values
atomsSetConfig("autoload",config_autoload);
atomsRepositorySetOfl("http://atoms.scilab.org");
