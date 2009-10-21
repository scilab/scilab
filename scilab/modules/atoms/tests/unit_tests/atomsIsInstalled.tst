// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// Do not use the autoload system
config_autoload = atomsGetConfig("autoload");
config_Verbose  = atomsGetConfig("Verbose");
atomsSetConfig("autoload","False");
atomsSetConfig("Verbose" ,"True");

// Load the 2nd scenario : See scene2.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsGetTOOLBOXES(%T);

// Install toolbox N°2
atomsInstall(["toolbox_2","2.0";"toolbox_2","2.1"],"user");

// Check input parameters
if execstr("atomsIsInstalled()","errcatch") == 0 then pause; end
if execstr("atomsIsInstalled([""toolbox_1"",""1.0"";""toolbox_2"",""2.0""],[""1.0"";""2.0""]  )","errcatch") == 0 then pause, end

// Check output results
if ~ atomsIsInstalled("toolbox_1") then pause, end
if ~ atomsIsInstalled("toolbox_2") then pause, end

if ~ atomsIsInstalled(["toolbox_1","1.0"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.0"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.1"]) then pause, end

if atomsIsInstalled(["toolbox_1","1.0"],"allusers") then pause, end
if atomsIsInstalled(["toolbox_2","2.0"],"allusers") then pause, end
if atomsIsInstalled(["toolbox_2","2.1"],"allusers") then pause, end

if ~ atomsIsInstalled(["toolbox_1","1.0"],"user") then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.0"],"user") then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.1"],"user") then pause, end

if ~ atomsIsInstalled(["toolbox_1","1.0"],"all") then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.0"],"all") then pause, end
if ~ atomsIsInstalled(["toolbox_2","2.1"],"all") then pause, end

if or( atomsIsInstalled(["toolbox_1","1.0";"toolbox_2","0.0";"toolbox_2","2.1";"toolbox_99","1.0"]) <> [%T;%F;%T;%F] )  then pause, end

// Remove toolbox_5 & toolbox_3
// =============================================================================
atomsRemove("toolbox_2","user");

// Restore original values
// =============================================================================
atomsSetConfig("autoload",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl("http://atoms.scilab.org");
