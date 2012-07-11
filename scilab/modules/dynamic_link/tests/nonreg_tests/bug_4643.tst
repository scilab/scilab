// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4643 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4643
//
// <-- Short Description -->
// ilib_build_f90.tst fails on windows x64 debub mode and not with release

// build with debug configuration and launch ilib_build_f90.tst
exec SCI/modules/dynamic_link/tests/unit_tests/ilib_build_f90.tst

// build with release configuration and launch ilib_build_f90.tst
exec SCI/modules/dynamic_link/tests/unit_tests/ilib_build_f90.tst
