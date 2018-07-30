// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 13522 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13522
//
// <-- Short Description -->
// savematfile() failed for hypermatrices

testfile = fullfile(TMPDIR, "bug_13522.mat");

// decimal numbers
h = rand(2,3,4,5);
savematfile(testfile, "h", "-v7.3");
h0 = h;
clear h
loadmatfile(testfile);
assert_checkequal(h, h0);
deletefile(testfile);

// Complex numbers
h = h + h*%i;
savematfile(testfile, "h", "-v7.3");
h0 = h;
clear h
loadmatfile(testfile);
assert_checkequal(h, h0);
deletefile(testfile);

// Encoded integers
for it = [1 2 4 8 11 12 14 18]
    Min = max(double(iconvert(-%inf, it)), -(2^30));
    Max = min(double(iconvert(%inf, it)), 2^30);
    h = iconvert(grand(2,3,4,5,"uin",Min,Max), it);
    savematfile(testfile, "h", "-v7.3");
    h0 = h;
    clear h
    loadmatfile(testfile);
    assert_checkequal(h, h0);
    deletefile(testfile);
end

// Booleans: TO BE FIXED:
//    http://bugzilla.scilab.org/15567
//    http://bugzilla.scilab.org/15568
//h = rand(3,4,5,6)<0.5;
//h = rand(3,4)<0.5;
//savematfile(testfile, "h", "-v7.3");
//h0 = h;
//clear h
//loadmatfile(testfile);
//assert_checkequal(h, h0);
//deletefile(testfile);

// Texts: TO BE FIXED : http://bugzilla.scilab.org/15569
//h = string(rand(3,4,5,6));
//savematfile(testfile, "h", "-v7.3");
//h0 = h;
//clear h
//loadmatfile(testfile);
//assert_checkequal(h, h0);
//deletefile(testfile);

