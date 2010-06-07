// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

load("SCI/modules/atoms/macros/atoms_internals/lib");

// We need a clean version
// =============================================================================
if ~isempty( atomsGetInstalled() ) then pause, end 

// Set some parameters for the test
// =============================================================================
config_autoload = atomsGetConfig("autoloadAddAfterInstall");
config_Verbose  = atomsGetConfig("Verbose");
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.test.atoms.scilab.org");

// 1st test case
// =============================================================================

// Install toolbox_5 in the user section
atomsInstall("toolbox_5","user");

// atomsAutoloadAdd just after install is disable, 
// → atomsAutoloadList should return an empty matrix
if ~ isempty(atomsAutoloadList()) then pause, end

if atomsAutoloadAdd("toolbox_5"               ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0"]       ,"user") <> 0 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0" "user"],"user") <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_4" "1.0" "user"],"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_4" "1.0"]       ,"user") <> 0 then pause, end
if atomsAutoloadAdd("toolbox_4"               ,"user") <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_2" "1.0"]       ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_2" "1.0" "user"],"user") <> 0 then pause, end
if atomsAutoloadAdd("toolbox_2"               ,"user") <> 0 then pause, end

ref = ["toolbox_5" "1.0-1" "user" "user" ; ..
       "toolbox_4" "1.0-1" "user" "user" ; ..
       "toolbox_2" "1.0-1" "user" "user"];


if or(atomsAutoloadList()<>ref) then pause, end


atomsRemove("toolbox_5");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end 
if ~isempty( atomsAutoloadList() ) then pause, end 

// 2nd test case
// =============================================================================

// Install toolbox_5 in both user and allusers sections
atomsInstall("toolbox_5","allusers");

// atomsAutoloadAdd just after install is disable, 
// → atomsAutoloadList should return an empty matrix
if ~ isempty(atomsAutoloadList()) then pause, end

if atomsAutoloadAdd("toolbox_5"                   ,"allusers") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0"]           ,"allusers") <> 0 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0" "allusers"],"allusers") <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_4" "1.0" "allusers"],"allusers") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_4" "1.0"]           ,"allusers") <> 0 then pause, end
if atomsAutoloadAdd("toolbox_4"                   ,"allusers") <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_2" "1.0"]           ,"allusers") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_2" "1.0" "allusers"],"allusers") <> 0 then pause, end
if atomsAutoloadAdd("toolbox_2"                   ,"allusers") <> 0 then pause, end

ref = ["toolbox_5" "1.0-1" "allusers" "allusers" ; ..
       "toolbox_4" "1.0-1" "allusers" "allusers" ; ..
       "toolbox_2" "1.0-1" "allusers" "allusers"];

if or(atomsAutoloadList()<>ref) then pause, end

atomsAutoloadDel( ["toolbox_5";"toolbox_4";"toolbox_2"] );
if ~isempty( atomsAutoloadList() ) then pause, end 

if atomsAutoloadAdd("toolbox_5"                   ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0"]           ,"user") <> 0 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0" "allusers"],"user") <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_4" "1.0" "allusers"],"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_4" "1.0"]           ,"user") <> 0 then pause, end
if atomsAutoloadAdd("toolbox_4"                   ,"user") <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_2" "1.0"]           ,"user") <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_2" "1.0" "allusers"],"user") <> 0 then pause, end
if atomsAutoloadAdd("toolbox_2"                   ,"user") <> 0 then pause, end

ref = ["toolbox_5" "1.0-1" "allusers" "user" ; ..
       "toolbox_4" "1.0-1" "allusers" "user" ; ..
       "toolbox_2" "1.0-1" "allusers" "user"];

if or(atomsAutoloadList()<>ref) then pause, end

atomsAutoloadDel( ["toolbox_5";"toolbox_4";"toolbox_2"] );
if ~isempty( atomsAutoloadList() ) then pause, end


if atomsAutoloadAdd("toolbox_5"                   ) <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0"]           ) <> 0 then pause, end
if atomsAutoloadAdd(["toolbox_5" "1.0" "allusers"]) <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_4" "1.0" "allusers"]) <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_4" "1.0"]           ) <> 0 then pause, end
if atomsAutoloadAdd("toolbox_4"                   ) <> 0 then pause, end

if atomsAutoloadAdd(["toolbox_2" "1.0"]           ) <> 1 then pause, end
if atomsAutoloadAdd(["toolbox_2" "1.0" "allusers"]) <> 0 then pause, end
if atomsAutoloadAdd("toolbox_2"                   ) <> 0 then pause, end

ref = ["toolbox_5" "1.0-1" "allusers" "allusers" ; ..
       "toolbox_4" "1.0-1" "allusers" "allusers" ; ..
       "toolbox_2" "1.0-1" "allusers" "allusers"];

if or(atomsAutoloadList()<>ref) then pause, end

atomsRemove("toolbox_5");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end 

// Restore original values
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));
