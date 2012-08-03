// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9570 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9570
//
// <-- Short Description -->
// fprintfMat did not use the good format argument with nan and inf in a matrix
//

tpmfile = fullfile(TMPDIR, "bug_9570_1.csv");
data = ones(3,2)./3;;
data(2,1) = %nan;
comments = "col1" + ascii(9) + "col2" + ascii(9);
fprintfMat(tpmfile, data, "%0.2f" + ascii(9), comments);

txt = mgetl(tpmfile);
ref = "Nan" + ascii(9) + " " + sprintf("%0.2f", data(2,2)) + ascii(9) + " ";
assert_checkequal(txt(3,1), ref)

[a, b] = fscanfMat(tpmfile);
assert_checkequal(b, comments);
assert_checkalmostequal(a, data, 10e-3);

tpmfile = fullfile(TMPDIR, "bug_9570_2.csv");
data = ones(3,2);
data(2,1) = -%inf;
data(2,2) = %inf;
fprintfMat(tpmfile, data, "%0.2f" + ascii(9), comments);

txt = mgetl(tpmfile);
ref = "-Inf" + ascii(9) + " " + "Inf" + ascii(9) + " ";
assert_checkequal(txt(3,1), ref);
