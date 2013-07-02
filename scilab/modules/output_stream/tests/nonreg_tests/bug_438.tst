// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 438 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=438
//
// <-- Short Description -->
// dummy =sprintf('%e', 1000) buggy 

dummy = sprintf('%e', 100);
assert_checkequal(dummy, "1.000000e+02");
  
dummy2 =sprintf('%e', 1000);
assert_checkequal(dummy2, "1.000000e+03");

dummy3 =sprintf('%e', 10000);
assert_checkequal(dummy3, "1.000000e+04");
