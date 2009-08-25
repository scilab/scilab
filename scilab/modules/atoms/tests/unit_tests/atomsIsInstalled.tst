// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// Load the 2nd scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");

// Install toolbox N°2
atomsInstall("toolbox_2 2.0");
atomsInstall("toolbox_2 2.1");
atomsInstall("toolbox_3 1.0");

// Check input parameters
if execstr("atomsIsInstalled()","errcatch") == 0 then pause; end
if execstr("atomsIsInstalled([""toolbox_1"" ""toolbox_2""],[""1.0"";""2.0""]  )","errcatch") == 0 then pause, end
if execstr("atomsIsInstalled([""toolbox_1"" ""toolbox_1""],[""1.0""]  )","errcatch") == 0 then pause, end

// Check output results
if ~ atomsIsInstalled("toolbox_1") then pause, end
if ~ atomsIsInstalled("toolbox_2") then pause, end

if ~ atomsIsInstalled("toolbox_1","1.0") then pause, end
if ~ atomsIsInstalled("toolbox_2","2.0") then pause, end
if ~ atomsIsInstalled("toolbox_2","2.1") then pause, end

if or( atomsIsInstalled(["toolbox_1" "toolbox_2" "toolbox_2" "toolbox_99"],["1.0" "0.0" "2.1" "1.0"] ) <> [%T %F %T %F] )  then pause, end
if or( atomsIsInstalled(["toolbox_1" "toolbox_2";"toolbox_2" "toolbox_99"],["1.0" "0.0";"2.1" "1.0"] ) <> [%T %F;%T %F] )  then pause, end
if or( atomsIsInstalled(["toolbox_1";"toolbox_2";"toolbox_2";"toolbox_99"],["1.0";"0.0";"2.1";"1.0"] ) <> [%T;%F;%T;%F] )  then pause, end
