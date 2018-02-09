// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 14531 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14531
//
// <-- Short Description -->
// Allow SCIHOME to be specified by user (at start-up)

//start scilab like

//windows
bin\scilab -nwni -scihome %USERPROFILE%\Scilab\new_home

//linux
bin/scilab -nwni -scihome ~/Scilab/new_home

//check that SCIHOME shows good directory