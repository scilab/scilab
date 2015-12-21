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

// Do not use the autoload system
// =============================================================================
atomsSetConfig("autoloadAddAfterInstall","False");
atomsSetConfig("Verbose" ,"False");


// 1st test-case : Just install the toolbox 5
// =============================================================================

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.6.0.test.atoms.scilab.org");

atomsInstall("toolbox_5V6","user");

// Check if the module is really installed

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.1-1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene11.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene11.6.0.test.atoms.scilab.org");

atomsUpdate();

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.0-1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Remove the module
atomsRemove("toolbox_5V6","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end

// 2nd test-case : Install toolbox 4 and then toolbox 5
// =============================================================================

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.6.0.test.atoms.scilab.org");

atomsInstall("toolbox_4V6","user");
atomsInstall("toolbox_5V6","user");

// Check if the module is really installed

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.1-1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene11.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene11.6.0.test.atoms.scilab.org");

atomsUpdate();

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.0-1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Remove the module
atomsRemove("toolbox_5V6","user");
atomsRemove("toolbox_4V6","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end

// 3rd test-case : Install toolbox 4 and toolbox 3
// => Test the behaviour of toolbox_2
// =============================================================================

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.6.0.test.atoms.scilab.org");

atomsInstall("toolbox_3V6","user");
atomsInstall("toolbox_4V6","user");

if ~atomsIsInstalled(["toolbox_4V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_3V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "2.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if  atomsIsInstalled(["toolbox_4V6" "1.1-1"],"all")  then pause, end
if  atomsIsInstalled(["toolbox_2V6" "2.1-1"],"all")  then pause, end

if atomsGetInstalledStatus(["toolbox_4V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_3V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "2.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene11.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene11.6.0.test.atoms.scilab.org");

atomsUpdate();

if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_3V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "2.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if  atomsIsInstalled(["toolbox_4V6" "1.0-1"],"all")  then pause, end
if  atomsIsInstalled(["toolbox_2V6" "2.0-1"],"all")  then pause, end

if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_3V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "2.1-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

atomsRemove("toolbox_4V6","user");
atomsRemove("toolbox_3V6","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end

// 4th test-case : Test the user/allusers behaviour
// =============================================================================

// Load the 1st scenario : See scene10.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene10.6.0.test.atoms.scilab.org");

atomsInstall("toolbox_5V6","user");
atomsInstall("toolbox_5V6","allusers");

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.0-1"],"allusers") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.1-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"allusers") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"allusers")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.0-1"],"allusers")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"allusers")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"allusers")<>"A" then pause, end

// Load the 2nd scenario : See scene11.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene11.6.0.test.atoms.scilab.org");

atomsUpdate([],"user");

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.0-1"],"allusers") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.1-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"allusers") then pause, end

if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.0-1"],"allusers")<>"A" then pause, end

atomsUpdate([],"allusers");

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"allusers") then pause, end
if  atomsIsInstalled(["toolbox_4V6" "1.0-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"allusers") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"allusers") then pause, end

if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"allusers")<>"A" then pause, end

atomsRemove("toolbox_5V6","user");
atomsRemove("toolbox_5V6","allusers");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end

// 5th test-case : Test the packaging version update
// =============================================================================

// Load the 1st scenario : See scene11.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene11.6.0.test.atoms.scilab.org");

atomsInstall("toolbox_5V6","user");

// Check if the module is really installed

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"all")  then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Load the 2nd scenario : See scene12.test.atoms.scilab.org.txt
atomsRepositorySetOfl("http://scene12.6.0.test.atoms.scilab.org");

atomsUpdate();

if ~atomsIsInstalled(["toolbox_5V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_4V6" "1.1-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_2V6" "1.0-1"],"user") then pause, end
if ~atomsIsInstalled(["toolbox_1V6" "1.0-1"],"user") then pause, end

if atomsGetInstalledStatus(["toolbox_5V6" "1.0-1"],"user")<>"I" then pause, end
if atomsGetInstalledStatus(["toolbox_4V6" "1.1-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_2V6" "1.0-1"],"user")<>"A" then pause, end
if atomsGetInstalledStatus(["toolbox_1V6" "1.0-1"],"user")<>"A" then pause, end

// Remove the module
atomsRemove("toolbox_5V6","user");

// no module should be installed
if ~isempty( atomsGetInstalled() ) then pause, end

// Restore original values
// =============================================================================
atomsRestoreConfig(%T);

atomsRepositorySetOfl(mgetl(SCI+"/modules/atoms/tests/unit_tests/repositories.orig"));