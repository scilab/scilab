// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16325 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=16325
//
// <-- Short Description -->
// mgetl could not read text file with single line data which is greater than ~260,000 characters

//create file with 1 line
n = [1d0 1d1 1d2 1d3 1d4 1d5 1d6];

for i=n
    a = rand(1, i) * 10;
    a = a + ascii("0");
    filename = fullfile(TMPDIR, "16325.dat");
    f = mopen(filename, "wt");
    mput(a, "c", f);
    mclose(f);

    //read file
    x = mgetl(filename);
    assert_checkequal(length(x), i);
end
