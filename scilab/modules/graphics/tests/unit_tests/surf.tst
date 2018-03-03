// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Unit tests for surf() -->


[X,Y] = meshgrid(-1:.1:1,-1:.1:1);
Z = X.^2 - Y.^2;
wh = color("white");
ma = color("magenta");
orange = color("orange");

// Testing already supported global propeties => Back-compatibility
// ----------------------------------------------------------------
// Foreground
surf(X, Y, Z, "foreground", "magenta");
assert_checkequal(gce().foreground, ma);
clf
surf(X, Y, Z, "foreground", "ma");
assert_checkequal(gce().foreground, ma);
clf
surf(X, Y, Z, "foreground", [1 0 1]);
assert_checkequal(gce().foreground, ma);
clf
surf(X, Y, Z, "foreground", "none");
assert_checkequal(gce().color_mode, 0);
assert_checkequal(gce().mark_foreground, -1);

// facecolor
clf
surf(X, Y, Z, "facecolor", "none");
assert_checkequal(gce().surface_mode, "on");
assert_checkequal(gce().color_mode, 0);
assert_checkequal(gce().color_flag, 0);
clf
surf(X, Y, Z, "facecolor", "flat");
assert_checkequal(gce().surface_mode, "on");
assert_checkequal(gce().color_flag, 4);
clf
surf(X, Y, Z, "facecolor", "interp");
assert_checkequal(gce().surface_mode, "on");
assert_checkequal(gce().color_flag, 3);

// markforeground
clf
surf(X, Y, Z, "foreground", "magenta", "marker", "*", "markforeground", "auto");
assert_checkequal(gce().mark_foreground, ma);
clf
surf(X, Y, Z, "foreground", "magenta", "marker","*", "markforeground", "none");
assert_checkequal(gce().mark_foreground, gca().background);
clf
surf(X, Y, Z, "marker","*", "markforeground", "magenta");
assert_checkequal(gce().mark_foreground, ma);
clf
surf(X, Y, Z, "marker","*", "markforeground", "ma");
assert_checkequal(gce().mark_foreground, ma);
clf
surf(X, Y, Z, "marker","*", "markforeground", [1 0 1]);
assert_checkequal(gce().mark_foreground, ma);

// markbackground
clf     // NOT DOCUMENTED
surf(X, Y, Z, "foreground", "magenta", "marker", "*", "markbackground", "auto");
assert_checkequal(gce().mark_background, gca().background);
clf     // NOT DOCUMENTED
surf(X, Y, Z, "foreground", "magenta", "marker","*", "markbackground", "none");
assert_checkequal(gce().mark_background, gca().background);
clf
surf(X, Y, Z, "marker","*", "markbackground", "magenta");
assert_checkequal(gce().mark_background, ma);
clf
surf(X, Y, Z, "marker","*", "markbackground", "ma");
assert_checkequal(gce().mark_background, ma);
clf
surf(X, Y, Z, "marker","*", "markbackground", [1 0 1]);
assert_checkequal(gce().mark_background, ma);

// Testing Fixed Extended global properties (bug 15404)
// ----------------------------------------------------
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15404
// <-- Short Description -->
//  surf() (adn so mesh() that calls it) did not accept extended color names
//  out of a restricted list of 10 basic colors, nor color "RRGGBB" hexa codes,
//  nor color indices, for all their colored properties.
// Foreground
clf
surf(X, Y, Z, "foreground", "orange");
assert_checkequal(gce().foreground, color("orange"));
clf
surf(X, Y, Z, "foreground", "#FF00FF");
assert_checkequal(gce().foreground, color("magenta"));
clf
surf(X, Y, Z, "foreground", ma);
assert_checkequal(gce().foreground, ma);

// MarkForeground
clf
surf(X, Y, Z, "marker", "*", "markforeground", "orange");
assert_checkequal(gce().mark_foreground, orange);
clf
surf(X, Y, Z, "marker", "*", "markforeground", "#FF00FF");
assert_checkequal(gce().mark_foreground, ma);
clf
surf(X, Y, Z, "marker","*", "markforeground", ma);
assert_checkequal(gce().mark_foreground, ma);

// MarkBackground
clf
surf(X, Y, Z, "marker", "*", "markbackground", "orange");
assert_checkequal(gce().mark_background, orange);
clf
surf(X, Y, Z, "marker", "*", "markbackground", "#FF00FF");
assert_checkequal(gce().mark_background, ma);
clf
surf(X, Y, Z, "marker","*", "markbackground", ma);
assert_checkequal(gce().mark_background, ma);

