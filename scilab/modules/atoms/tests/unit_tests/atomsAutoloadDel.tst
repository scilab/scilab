// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// We need a clean version
// =============================================================================
if ~isempty( atomsGetInstalled() ) then pause, end

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

// Set some parameters for the test
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.6.0.test.atoms.scilab.org");

// 1st test case
// =============================================================================

// Install toolbox_5 in both user and allusers sections
atomsInstall("toolbox_5V6","user");

// atomsAutoloadAdd just after install is disable,
// → atomsAutoloadList should return an empty matrix
if ~ isempty(atomsAutoloadList()) then pause, end

if atomsAutoloadAdd(["toolbox_5V6" "1.0" "user"]    ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_4V6" "1.0" "user"]    ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_2V6" "1.0" "user"]    ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_1V6" "1.0" "user"]    ,"user") <> 1 then pause, end

if atomsAutoloadDel("toolbox_5V6"                   ,"user") <> 1 then pause, end
if atomsAutoloadDel(["toolbox_5V6" "1.0-1"]         ,"user") <> 0 then pause, end
if atomsAutoloadDel(["toolbox_5V6" "1.0-1" "user"]  ,"user") <> 0 then pause, end

if atomsAutoloadDel(["toolbox_4V6" "1.0-1" "user"]  ,"user") <> 1 then pause, end
if atomsAutoloadDel(["toolbox_4V6" "1.0-1"]         ,"user") <> 0 then pause, end
if atomsAutoloadDel("toolbox_4V6"                   ,"user") <> 0 then pause, end

if atomsAutoloadDel(["toolbox_2V6" "1.0-1"]         ,"user") <> 1 then pause, end
if atomsAutoloadDel(["toolbox_2V6" "1.0-1" "user"]  ,"user") <> 0 then pause, end
if atomsAutoloadDel("toolbox_2V6"                   ,"user") <> 0 then pause, end

if atomsAutoloadDel("toolbox_1V6")                           <> 1 then pause, end

if ~isempty( atomsAutoloadList() ) then pause, end

atomsRemove("toolbox_5V6");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end
if ~isempty( atomsAutoloadList() ) then pause, end

// 2nd test case
// =============================================================================

// Install toolbox_5 in both user and allusers sections
atomsInstall("toolbox_5V6","allusers");

// atomsAutoloadAdd just after install is disable,
// → atomsAutoloadList should return an empty matrix
if ~ isempty(atomsAutoloadList()) then pause, end

if atomsAutoloadAdd(["toolbox_5V6" "1.0" "allusers" ; ..
    "toolbox_4V6" "1.0" "allusers" ; ..
    "toolbox_2V6" "1.0" "allusers" ; ..
"toolbox_1V6" "1.0" "allusers" ] ,"allusers") <> 4 then pause, end

if atomsAutoloadDel("toolbox_5V6"                       ,"allusers") <> 1 then pause, end
if atomsAutoloadDel(["toolbox_5V6" "1.0-1"]             ,"allusers") <> 0 then pause, end
if atomsAutoloadDel(["toolbox_5V6" "1.0-1" "allusers"]  ,"allusers") <> 0 then pause, end

if atomsAutoloadDel(["toolbox_4V6" "1.0-1" "allusers"]  ,"allusers") <> 1 then pause, end
if atomsAutoloadDel(["toolbox_4V6" "1.0-1"]             ,"allusers") <> 0 then pause, end
if atomsAutoloadDel("toolbox_4V6"                       ,"allusers") <> 0 then pause, end

if atomsAutoloadDel(["toolbox_2V6" "1.0-1"]             ,"allusers") <> 1 then pause, end
if atomsAutoloadDel(["toolbox_2V6" "1.0-1" "allusers"]  ,"allusers") <> 0 then pause, end
if atomsAutoloadDel("toolbox_2V6"                       ,"allusers") <> 0 then pause, end

if atomsAutoloadDel("toolbox_1V6")                                   <> 1 then pause, end

if ~isempty( atomsAutoloadList() ) then pause, end

atomsRemove("toolbox_5V6");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end
if ~isempty( atomsAutoloadList() ) then pause, end


// 3rd test case
// =============================================================================

// Install toolbox_5 in both user and allusers sections
atomsInstall("toolbox_5V6","user");
atomsInstall("toolbox_5V6","allusers");

// atomsAutoloadAdd just after install is disable,
// → atomsAutoloadList should return an empty matrix
if ~ isempty(atomsAutoloadList()) then pause, end

if atomsAutoloadAdd(["toolbox_5V6" "1.0" "allusers"]    ,"allusers") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_5V6" "1.0" "user"]        ,"user")     <> 1 then pause, end

if atomsAutoloadDel("toolbox_5V6") <> 2 then pause, end

if atomsAutoloadAdd(["toolbox_5V6" "1.0" "allusers"]    ,"allusers") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_5V6" "1.0" "user"]        ,"user")     <> 1 then pause, end

if atomsAutoloadDel("toolbox_5V6","user")                            <> 1 then pause, end
if atomsAutoloadDel("toolbox_5V6","allusers")                        <> 1 then pause, end

if ~isempty( atomsAutoloadList() ) then pause, end

atomsRemove("toolbox_5V6","user");
atomsRemove("toolbox_5V6","allusers");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end
if ~isempty( atomsAutoloadList() ) then pause, end

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));