// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16717 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/16717
//
// <-- Short Description -->
// Error in findobj when no object exists

assert_checkequal(findobj(),get(0))
assert_checktrue(isempty(findobj("type","Figure")))

