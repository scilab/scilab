// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8390 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8390
//
// <-- Short Description -->
// a "Warning : redefining function: mputl" displayed if you did not install fftw on Windows.
//
//
// install scilab without fftw module
// check that you do not have this warning