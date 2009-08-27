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

// Install the toolbox 5
atomsInstall("toolbox_5");

