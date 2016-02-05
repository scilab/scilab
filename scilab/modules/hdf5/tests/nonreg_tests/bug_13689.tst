// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13689 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13689
//
// <-- Short Description -->
//
// There were errors in reading enum from hdf5 files

h5 = h5open(SCI + "/modules/hdf5/tests/nonreg_tests/bug_13689.h5");
assert_checkequal(h5.root.BooleanT.data, uint8(1));
assert_checkequal(h5.root.BooleanF.data, uint8(0));