// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - John Gliksberg
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// We need a clean version
// ============================================================================
if ~isempty( atomsGetInstalled() ) then pause, end

// If previous test did not end properly, restore, else backup config file
atomsRestoreConfig(%T);
atomsSaveConfig();

atomsInstall("toolbox_1V6");

atomsLoad("toolbox_1V6");

// Do the actual test_run
test_run("toolbox_1V6", "basic", "short_summary");

atomsRemove("toolbox_1V6");

// Restore original value
// ============================================================================
atomsRestoreConfig(%T);

