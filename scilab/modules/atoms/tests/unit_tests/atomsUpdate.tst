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


// 1st test-case : Just install the toolbox 5
// =============================================================================

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsSystemUpdate();

atomsInstall("toolbox_5","user");

// Check if the module is really installed

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene2.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsSystemUpdate();

atomsUpdate();

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.0"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

// Remove the module
atomsRemove("toolbox_5","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end 

// 2nd test-case : Install toolbox 4 and then toolbox 5
// =============================================================================

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsSystemUpdate();

atomsInstall("toolbox_4","user");
atomsInstall("toolbox_5","user");

// Check if the module is really installed

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene2.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsSystemUpdate();

atomsUpdate();

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.0"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

// Remove the module
atomsRemove("toolbox_5","user");
atomsRemove("toolbox_4","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end 

// 3rd test-case : Install toolbox 4 and toolbox 3
// => Test the behaviour of toolbox_2
// =============================================================================

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsSystemUpdate();

atomsInstall("toolbox_3","user");
atomsInstall("toolbox_4","user");

if ~atomsIsInstalled(["toolbox_4" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_3" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "2.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if  atomsIsInstalled(["toolbox_4" "1.1"],"all")  then pause, end
if  atomsIsInstalled(["toolbox_2" "2.1"],"all")  then pause, end

if atomsGetInstalledStatus(["toolbox_4" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_3" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "2.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene2.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsSystemUpdate();

atomsUpdate();

if ~atomsIsInstalled(["toolbox_4" "1.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_3" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "2.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if  atomsIsInstalled(["toolbox_4" "1.0"],"all")  then pause, end
if  atomsIsInstalled(["toolbox_2" "2.0"],"all")  then pause, end

if atomsGetInstalledStatus(["toolbox_4" "1.1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_3" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "2.1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

atomsRemove("toolbox_4","user");
atomsRemove("toolbox_3","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end 

// 4th test-case : Test the user/allusers behaviour
// =============================================================================

// Load the 1st scenario : See scene1.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene1.test.atoms.scilab.org");
atomsSystemUpdate();

atomsInstall("toolbox_5","user");
atomsInstall("toolbox_5","allusers");

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if ~atomsIsInstalled(["toolbox_5" "1.0"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.0"],"allusers") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"allusers") then pause, end

if atomsGetInstalledStatus(["toolbox_5" "1.0"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"user")<>"A" then pause, end

if atomsGetInstalledStatus(["toolbox_5" "1.0"],"allusers")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.0"],"allusers")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2" "1.0"],"allusers")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1" "1.0"],"allusers")<>"A" then pause, end

// Load the 2nd scenario : See scene2.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene2.test.atoms.scilab.org");
atomsSystemUpdate();

atomsUpdate([],"user");

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if ~atomsIsInstalled(["toolbox_5" "1.0"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.0"],"allusers") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"allusers") then pause, end

if atomsGetInstalledStatus(["toolbox_4" "1.1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.0"],"allusers")<>"A" then pause, end

atomsUpdate([],"allusers");

if ~atomsIsInstalled(["toolbox_5" "1.0"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"user") then pause, end

if ~atomsIsInstalled(["toolbox_5" "1.0"],"allusers") then pause, end
if  atomsIsInstalled(["toolbox_4" "1.0"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_4" "1.1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_2" "1.0"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_1" "1.0"],"allusers") then pause, end

if atomsGetInstalledStatus(["toolbox_4" "1.1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_4" "1.1"],"allusers")<>"A" then pause, end

atomsRemove("toolbox_5","user");
atomsRemove("toolbox_5","allusers");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end 

// Restore original values
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall",config_autoload);
atomsSetConfig("Verbose" ,config_Verbose);
atomsRepositorySetOfl("http://atoms.scilab.org");
