// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - John GLIKSBERG
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 13944 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13944
//
// <-- Short Description -->
// When there exists a contrib/ package which is not autoloaded, the Toolbox
// top-bar menu should be created.

// Install a module via atomsIntall or atomsGui

// Un-Autoload it via atomAutoloadDel or atomsGui

// Restart Scilab and make sure the Toolbox top-bar menu is available
