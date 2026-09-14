// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14202 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/14202
//
// <-- Short Description -->
// Logscale made Scilab crash (infinite loop) when using extreme bounds.

x = [1, 2];
y = [1e-305, 1e305];
plot(x, y, "b-");
gca().log_flags = "nln";
