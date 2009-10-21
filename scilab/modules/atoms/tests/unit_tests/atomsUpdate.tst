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
atomsGetTOOLBOXES(%T);

// Do not use the autoload system
config_autoload = atomsGetConfig("autoload");
atomsSetConfig("autoload","False");


// Install the toolbox 5
// =============================================================================

atomsInstall("toolbox_5");

// Check if the module is really installed
if ~atomsIsInstalled(["toolbox_5" "1.0"]) then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.0"]) then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"]) then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"]) then pause, end

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsGetTOOLBOXES(%T);

atomsUpdate();

if ~atomsIsInstalled(["toolbox_5" "1.0"]) then pause, end

if  atomsIsInstalled(["toolbox_4" "1.0"]) then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.1"]) then pause, end

if ~atomsIsInstalled(["toolbox_2" "1.0"]) then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"]) then pause, end

// Remove the module
atomsRemove("toolbox_5");
