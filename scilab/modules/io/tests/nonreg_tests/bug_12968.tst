//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
// Copyright (C) 2013 - Test case - Luc GABRIEL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12968 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12968
//
// <-- Short Description -->
//  Cannot save variable with name "temp"

temp = 0:10;
tempRef = temp;
save(TMPDIR + "/save.bin", "temp");
clear temp
load(TMPDIR + "/save.bin");
assert_checkequal(temp, tempRef);

temp = 0:10;
dens = 10:-1:0;
tempRef = temp;
densRef = dens;
save(TMPDIR + "/save.bin", "temp", "dens");
clear temp dens
load(TMPDIR + "/save.bin");
assert_checkequal(temp, tempRef);
assert_checkequal(dens, densRef);

temp = 0:10;
dens = 10:-1:0;
tempRef = temp;
densRef = dens;
save(TMPDIR + "/save.bin", "dens", "temp");
clear temp dens
load(TMPDIR + "/save.bin");
assert_checkequal(temp, tempRef);
assert_checkequal(dens, densRef);
