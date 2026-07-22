// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17567 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17567
//
// <-- Short Description -->
// kmeans returned a dimension mismatch error

K = csvRead(fullfile(SCI, "modules", "statistics", "tests", "nonreg_tests", "issue_17567.csv"));
m = 6;
for i = 1:10
    [idx, centroid] = kmeans(K, m);
    assert_checkequal(size(centroid), [6 2]);
end

