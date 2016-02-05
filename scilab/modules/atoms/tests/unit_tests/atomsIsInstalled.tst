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

// Load the 2nd scenario : See scene11.test.atoms.scilab.org.txt
// =============================================================================
atomsRepositorySetOfl("http://scene11.6.0.test.atoms.scilab.org");

// Install toolbox N°2
atomsInstall(["toolbox_2V6","2.0";"toolbox_2V6","2.1"],"user");

// Check input parameters
if execstr("atomsIsInstalled()","errcatch") == 0 then pause; end
if execstr("atomsIsInstalled([""toolbox_1V6"",""1.0"";""toolbox_2V6"",""2.0""],[""1.0"";""2.0""]  )","errcatch") == 0 then pause, end
if execstr("atomsIsInstalled([""toolbox_1V6"",""1.0"";""toolbox_2V6"",""2.0""],[""1.0"";""2.0""]  )","errcatch") == 0 then pause, end

// Check output results
if ~ atomsIsInstalled("toolbox_1V6") then pause, end
if ~ atomsIsInstalled("toolbox_2V6") then pause, end

if ~ atomsIsInstalled(["toolbox_1V6","1.0"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.0"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1"]) then pause, end

if ~ atomsIsInstalled(["toolbox_1V6","1.0-1"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.0-1"]) then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1-1"]) then pause, end

if atomsIsInstalled(["toolbox_1V6","1.0"],"allusers") then pause, end
if atomsIsInstalled(["toolbox_2V6","2.0"],"allusers") then pause, end
if atomsIsInstalled(["toolbox_2V6","2.1"],"allusers") then pause, end

if atomsIsInstalled(["toolbox_1V6","1.0-1"],"allusers") then pause, end
if atomsIsInstalled(["toolbox_2V6","2.0-1"],"allusers") then pause, end
if atomsIsInstalled(["toolbox_2V6","2.1-1"],"allusers") then pause, end

if ~ atomsIsInstalled(["toolbox_1V6","1.0"],"user") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.0"],"user") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1"],"user") then pause, end

if ~ atomsIsInstalled(["toolbox_1V6","1.0-1"],"user") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.0-1"],"user") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1-1"],"user") then pause, end

if ~ atomsIsInstalled(["toolbox_1V6","1.0"],"all") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.0"],"all") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1"],"all") then pause, end

if ~ atomsIsInstalled(["toolbox_1V6","1.0-1"],"all") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.0-1"],"all") then pause, end
if ~ atomsIsInstalled(["toolbox_2V6","2.1-1"],"all") then pause, end

if or( atomsIsInstalled(["toolbox_1V6","1.0";"toolbox_2V6","0.0";"toolbox_2V6","2.1";"toolbox_99V6","1.0"]) <> [%T;%F;%T;%F] )  then pause, end

A = ["toolbox_1V6","1.0" "allusers" ;
"toolbox_2V6","2.0" "allusers" ;
"toolbox_2V6","2.1" "allusers" ;
"toolbox_1V6","1.0" "user"     ;
"toolbox_2V6","2.0" "user"     ;
"toolbox_2V6","2.1" "user"     ;
"toolbox_1V6","1.0" "all"      ;
"toolbox_2V6","2.0" "all"      ;
"toolbox_2V6","2.1" "all"      ;
"toolbox_1V6","1.0" ""         ;
"toolbox_2V6","2.0" ""         ;
"toolbox_2V6","2.1" ""         ];

B = ["toolbox_1V6","1.0-1" "allusers" ;
"toolbox_2V6","2.0-1" "allusers" ;
"toolbox_2V6","2.1-1" "allusers" ;
"toolbox_1V6","1.0-1" "user"     ;
"toolbox_2V6","2.0-1" "user"     ;
"toolbox_2V6","2.1-1" "user"     ;
"toolbox_1V6","1.0-1" "all"      ;
"toolbox_2V6","2.0-1" "all"      ;
"toolbox_2V6","2.1-1" "all"      ;
"toolbox_1V6","1.0-1" ""         ;
"toolbox_2V6","2.0-1" ""         ;
"toolbox_2V6","2.1-1" ""         ];

if or( atomsIsInstalled(A,"all")      <> [ %F ; %F ; %F ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ]) then pause, end
if or( atomsIsInstalled(A,"user")     <> [ %F ; %F ; %F ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ]) then pause, end
if or( atomsIsInstalled(A,"allusers") <> [ %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ]) then pause, end

if or( atomsIsInstalled(B,"all")      <> [ %F ; %F ; %F ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ]) then pause, end
if or( atomsIsInstalled(B,"user")     <> [ %F ; %F ; %F ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ; %T ]) then pause, end
if or( atomsIsInstalled(B,"allusers") <> [ %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ; %F ]) then pause, end

// Remove toolbox_5 & toolbox_3
// =============================================================================
atomsRemove("toolbox_2V6","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
