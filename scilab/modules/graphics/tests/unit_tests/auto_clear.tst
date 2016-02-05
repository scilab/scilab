// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test auto_clear behaviour

a = gca();
a.auto_clear = "on";

plot3d();

assert_checkequal(size(a.children), [1 1]);
assert_checkequal(a.children(1).type, "Plot3d");

plot2d();
assert_checkequal(size(a.children), [1 1]);
assert_checkequal(a.children(1).type, "Compound");

xpoly([0 1], [0 1]);
assert_checkequal(size(a.children), [1 1]);
assert_checkequal(a.children(1).type, "Polyline");
