// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->



load("SCI/modules/atoms/macros/atoms_internals/lib");

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

// Do not use the autoload system
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

//force official ATOMS repository
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));

// Install toolbox N°2
atomsInstall(["toolbox_2V6","2.1"],"user");

// Check installation
if ~ atomsIsInstalled(["toolbox_1V6","2.0","user"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1","user"]) then pause, end

atomsLoad(["toolbox_2V6","2.1","user"]);

if ~ atomsIsLoaded("toolbox_2V6")                  then pause, end
if ~ atomsIsLoaded(["toolbox_2V6","2.1"])          then pause, end
if ~ atomsIsLoaded(["toolbox_2V6","2.1","user"])   then pause, end

if ~ atomsIsLoaded("toolbox_1V6")                  then pause, end
if ~ atomsIsLoaded(["toolbox_1V6","2.0"])          then pause, end
if ~ atomsIsLoaded(["toolbox_1V6","2.0","user"])   then pause, end

if atomsIsLoaded(["toolbox_1V6","2.0","allusers"]) then pause, end
if atomsIsLoaded(["toolbox_2V6","2.1","allusers"]) then pause, end

A = [ "toolbox_1V6" "2.0" "user"     ; ..
"toolbox_2V6" "2.1" "user"     ; ..
"toolbox_2V6" ""    "user"     ; ..
"toolbox_2V6" "2.1" ""         ; ..
"toolbox_2V6" ""    ""         ; ..
"toolbox_2V6" "2.1" "allusers" ];

if or(atomsIsLoaded(A) <> [%T ; %T ; %T ; %T ; %T ; %F]) then pause, end

// Remove toolbox_2
// =============================================================================
atomsRemove("toolbox_2V6","user");

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);
