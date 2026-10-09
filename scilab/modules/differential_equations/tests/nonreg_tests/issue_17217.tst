// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- Non-regression test for issue 17217 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17217
//
// <-- Short Description -->
// Calling impl with wrong input size crashes Scilab.

msgerr = msprintf(gettext("%s: Wrong size for input argument #%d and #%d: Same size expected.\n"), "%_impl", 2, 3);
assert_checkerror("y = %_impl(""stiff"", [1;0], [-0.04;0.04;0], 0, 0.4, ""resid"", ""aplusp"");", msgerr);
