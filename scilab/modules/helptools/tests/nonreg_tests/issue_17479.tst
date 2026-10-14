// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK ERROR OUTPUT -->
//
// <-- Non-regression test for bug 17479 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17479
//
// <-- Short Description -->
// inline doc fails for non existing language documentation
//

setlanguage("en_PH");
err = execstr("help cos", "errcatch");
assert_checkequal(err, 0);
