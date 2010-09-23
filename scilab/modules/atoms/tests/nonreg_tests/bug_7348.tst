// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7348 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7348
//
// <-- Short Description -->
// there is the possibility to install 2 times same modules as 'user' and 'allusers'

atomsInstall(["module_lycee" "1.2"],"user");
atomsInstall(["module_lycee" "1.2"],"allusers");

// Restart Scilab
// "module_lycee" should only load one time

atomsRemove(["module_lycee" "1.2"]);
