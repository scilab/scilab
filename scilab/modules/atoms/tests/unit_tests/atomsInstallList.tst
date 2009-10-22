// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
// =============================================================================

atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsGetTOOLBOXES(%T);

// -----------------------------------------------------------------------------

ref = [ "+" "U" "toolbox_4" "1.0" ;
        "+" ""  "toolbox_2" "1.0" ;
        "+" ""  "toolbox_1" "1.0" ];

if or(atomsInstallList("toolbox_4")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "+" "U" "toolbox_3" "1.0" ;
        "+" ""  "toolbox_2" "2.0" ;
        "+" ""  "toolbox_1" "1.0" ];

if or(atomsInstallList("toolbox_3")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "+" "U" "toolbox_6" "1.0" ;
        "+" ""  "toolbox_2" "2.0" ;
        "+" ""  "toolbox_1" "1.0" ];

if or(atomsInstallList("toolbox_6")<>ref) then pause, end



// Load the 2nd scenario : See scene1.test.atoms.scilab.org.txt
// =============================================================================

atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsGetTOOLBOXES(%T);

// -----------------------------------------------------------------------------

ref = [ "+" "U" "toolbox_4" "1.1" ;
        "+" ""  "toolbox_2" "1.0" ;
        "+" ""  "toolbox_1" "1.0" ];

if or(atomsInstallList("toolbox_4")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "+" "U" "toolbox_3" "1.0" ;
        "+" ""  "toolbox_2" "2.1" ;
        "+" ""  "toolbox_1" "1.0" ];

if or(atomsInstallList("toolbox_3")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "+" "U" "toolbox_6" "1.0" ;
        "+" ""  "toolbox_2" "2.1" ;
        "+" ""  "toolbox_1" "1.0" ];

if or(atomsInstallList("toolbox_6")<>ref) then pause, end



// Restore Original values
// =============================================================================
atomsRepositorySetOfl("http://atoms.scilab.org");
atomsGetTOOLBOXES(%T);
