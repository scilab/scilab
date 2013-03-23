// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12391 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12391
//
// <-- Short Description -->
// findobj could change current figure returned by gcf();

f = gcf(); // Figure #0
f.tag = "currentfigure";

g = scf(); // Figure #1
g.tag = "otherfigure";

scf(f);

findobj("tag", "otherfigure");

assert_checkequal(get(gcf(), "tag"), "currentfigure");