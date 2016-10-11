// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9876 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9876
//
// <-- Short Description -->
// Struct creation by element insertion failed.

opencv_storage.haarcascade_alt.stages(1).trees(1).value(1).feature.rects(1).value = [3 7 14 4 -1.];
opencv_storage.haarcascade_alt.stages(1).trees(2).value(1).feature.rects(1).value = [1 2 18 4 -1.];

assert_checkequal(opencv_storage.haarcascade_alt.stages(1).trees(1).value(1).feature.rects(1).value, [3 7 14 4 -1.]);
assert_checkequal(opencv_storage.haarcascade_alt.stages(1).trees(2).value(1).feature.rects(1).value, [1 2 18 4 -1.]);
