// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Clément DAVID
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- XCOS TEST --> 
//
// <-- Non-regression test for bug 16343 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/16343
//
// <-- Short Description -->
// Import of a Scilab/Xcos 5.5.2 model resized the blocks poorly
//

scs_m = xcosDiagramToScilab(fullfile(SCI, "modules", "xcos", "tests", "nonreg_tests", "issue_16343.xcos"));

for o = scs_m.objs
    if typeof(o) == "Block" then
        geom = [o.graphics.orig o.graphics.sz];
        // sz should be multiple of 10
        assert_checkequal(round(o.graphics.sz / 10) * 10, o.graphics.sz);
        // should be aligned on grid
        assert_checkequal(round(geom / 10) * 10, geom);
    elseif typeof(o) == "Text" then
        // check text
        assert_checktrue(o.graphics.exprs(1) <> "Text");
        assert_checkequal(o.graphics.exprs(2:3), ["2";"1"]);
    end
end
