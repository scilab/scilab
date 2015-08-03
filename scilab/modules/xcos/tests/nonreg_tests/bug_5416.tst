/ =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 5416 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5416
//
// <-- Short Description -->
// If one modifies a value inside a superblock, the change is not propagated
// to the whole diagram.



// xcos(SCI + '/modules/xcos/demos/Discrete-KalmanFilter.zcos');
// Open a superblock
// modify value inside
// check that the simulation differs when values change



