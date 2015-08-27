// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 10560 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10560
//
// <-- Short Description -->
// genetic algorithms demos failed

ierr = exec("SCI/modules/genetic_algorithms/demos/GAdemo.sce", "errcatch", -1);
assert_checkequal(ierr, 0);
