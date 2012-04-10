// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 10946 -->

// <-- Short Description -->
// load function (hdf5 based) cannot load a tlist.
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10946
//

// Try to load ref file
lst_reference  =  tlist(['random numbers';'Name';'Example'], 'Uniform',ones(1,2, 3));
import_from_hdf5(SCI+"/modules/hdf5/tests/sample_scilab_data/tlist.h5");
assert_checkequal(lst,lst_reference);

// Try to save the tlist
export_to_hdf5(TMPDIR + filesep() + "bug_10946.sod", "lst");
clear lst
import_from_hdf5(TMPDIR + filesep() + "bug_10946.sod");
assert_checkequal(lst,lst_reference);

