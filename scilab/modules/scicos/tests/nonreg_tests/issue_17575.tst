// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17575 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17575
// 
// <-- Non-regression test for bug 17575 -->
//
// <-- Short Description -->
// An SSP file makes Scilab crash 
//

loadXcosLibs
scicos_log("INFO")

scs_m = scicosDiagramToScilab("SCI/modules/scicos/tests/nonreg_tests/issue_17575.ssp");
