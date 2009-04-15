// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4150 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4150
//
// <-- Short Description -->
// SCIHOME\scilab.ini is executed twice at Scilab startup.
// with cd(getenv('SCIHOME')) SCIHOME\scilab.ini

// <-- INTERACTIVE TEST -->

// go to in your SCIHOME directory
// creates a .scilab or scilab.ini file
// put in this file 

printf('go to SCILAB working directory\n\n');
cd(SCIHOME);

// saves .scilab
// and launch scilab

// Check that you have only ONE 'go to SCILAB working directory'



