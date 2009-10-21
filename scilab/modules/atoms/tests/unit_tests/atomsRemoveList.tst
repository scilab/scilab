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

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsGetTOOLBOXES(%T);

// Install toolbox_5 & toolbox_3
// =============================================================================
atomsInstall("toolbox_5","user");
atomsInstall("toolbox_3","user");

// -----------------------------------------------------------------------------

ref = [ "-" "U" "toolbox_1" "1.0" "user" ;
        "-" "P" "toolbox_5" "1.0" "user" ;
        "-" "P" "toolbox_4" "1.0" "user" ;
        "-" "P" "toolbox_2" "1.0" "user" ;
        "-" "P" "toolbox_3" "1.0" "user" ;
        "-" "P" "toolbox_2" "2.0" "user" ];

if or(atomsRemoveList(["toolbox_1"]      ,"user")<>ref) then pause, end
if or(atomsRemoveList(["toolbox_1" "1.0"],"user")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "-" "U" "toolbox_2" "1.0" "user" ;
        "-" "P" "toolbox_5" "1.0" "user" ;
        "-" "P" "toolbox_4" "1.0" "user" ;
        "~" "C" "toolbox_1" "1.0" "user" ];

if or(atomsRemoveList(["toolbox_2" "1.0"],"user")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "-" "U" "toolbox_2" "2.0" "user" ;
        "-" "P" "toolbox_3" "1.0" "user" ;
        "~" "C" "toolbox_1" "1.0" "user" ];

if or(atomsRemoveList(["toolbox_2" "2.0"],"user")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "-" "U" "toolbox_2" "2.0" "user" ;
        "-" "U" "toolbox_2" "1.0" "user" ;
        "-" "P" "toolbox_3" "1.0" "user" ;
        "-" "C" "toolbox_1" "1.0" "user" ;
        "-" "P" "toolbox_5" "1.0" "user" ;
        "-" "P" "toolbox_4" "1.0" "user" ];

if or(atomsRemoveList(["toolbox_2"],"user")<>ref) then pause, end

// -----------------------------------------------------------------------------

ref = [ "-" "U" "toolbox_2" "2.0" "user" ;
        "-" "U" "toolbox_2" "1.0" "user" ;
        "-" "U" "toolbox_4" "1.0" "user" ;
        "-" "P" "toolbox_3" "1.0" "user" ;
        "-" "C" "toolbox_1" "1.0" "user" ;
        "-" "P" "toolbox_5" "1.0" "user" ];

if or(atomsRemoveList(["toolbox_2";"toolbox_4"],"user")<>ref) then pause, end


// Remove toolbox_5 & toolbox_3
// =============================================================================
atomsRemove("toolbox_5","user");
atomsRemove("toolbox_3","user");

// Restore original values
// =============================================================================
atomsSetConfig("autoload",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl("http://atoms.scilab.org");
