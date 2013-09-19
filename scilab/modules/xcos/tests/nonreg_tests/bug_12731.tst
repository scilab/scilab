// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 12731 -->
//
// <-- INTERACTIVE TEST -->

// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12731
//
// <-- Short Description -->
// Code generation produces erroneous block
atomsInstall("Coselica")
xcos(SCI + "/modules/xcos/tests/nonreg_tests/bug_12731.zcos");

//now run the simulation, then generate code for the Asservissement super_block and run simulation again

