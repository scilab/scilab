// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 7212 -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7212
//
// <-- Short Description -->
// 
// Start Demos. --> Xcos --> Modelica demos --> RLC circuit
// launch demo
// edit Makefile generated in TMPDIR and check that cflags as includes path 
// are good.

// Previously on Windows,
// include paths were false and long pathname not managed
