// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- Non-regression test for bug 3909 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3909
//
// <-- Short Description -->
// error R6034 at startup when you launch tests on Windows x64

// <-- INTERACTIVE TEST -->
// <-- WINDOWS ONLY -->

if win64() <> %t then pause,end

// launch all others tests of javasci
// another way is to check SCI/modules/core/examples/call_scilab/c/visual_studio example

