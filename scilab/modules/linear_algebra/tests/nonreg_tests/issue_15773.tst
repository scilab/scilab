// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug  -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/15773
//
// <-- Short Description -->
// eigs dit not compute the eigenvalues of a singular matrix
//

a = sprand(10,10,0.5);
a(:,1) = 0;
d = eigs(a, [], 2, 'SM');
d0 = spec(full(a));
assert_checkalmostequal(d, d0([7 1]), [], 1.4e-16);
