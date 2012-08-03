// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 10864 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10864
//
// <-- Short Description -->
// atomsRemove required a network connection to remove a package.

// download a module
// disable your network connection 

atomsSetConfig('offline', 'false')
atomsInstall('~/grocer_1.52-1.bin.zip')
atomsRemove('grocer')
atomsSetConfig('offline', 'true')

