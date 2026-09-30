// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NOT FIXED -->
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Non-regression test for issue 16737 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/16737
//
// <-- Short Description -->
// xsave/xload craches scilab 6.1.1 when saving a figure with GUI

xload(fullfile(SCI, "modules", "graphics", "tests", "nonreg_tests", "issue_16737_522.scg"));
delete(gcf());
xload(fullfile(SCI, "modules", "graphics", "tests", "nonreg_tests", "issue_16737_611.scg"));
delete(gcf());
