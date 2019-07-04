// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14660 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14660
//
// <-- Short Description -->
// dec2base is 53-bit limited by double and not uint64 ready

assert_checkequal(dec2base(uint64(%inf),36),"3W5E11264SGSF");
assert_checkequal(dec2base(uint64(%inf),2),"1111111111111111111111111111111111111111111111111111111111111111");
msg = msprintf(_("%s: Wrong value for input argument #%d: Must be between 0 and 2^53.\n"), "dec2base", 1);
assert_checkerror("dec2base(2^53+2,2)", msg);