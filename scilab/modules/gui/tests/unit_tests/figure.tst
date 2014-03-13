// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Check figure function behaviour

f = figure();
a = gca();
assert_checkequal(f.background, 33);
assert_checkequal(a.background, f.background);
assert_checkequal(a.filled, "off");
assert_checkequal(size(f.color_map), [33, 3]);
delete(f);

f = figure("default_axes", "off");
assert_checkequal(f.children, []);
delete(f);

// Existing color
f = figure("backgroundcolor", [0 1 0]);
a = gca();
assert_checkequal(f.background, 3);
assert_checkequal(a.background, 33);
assert_checkequal(a.filled, "off");
assert_checkequal(size(f.color_map), [33, 3]);
assert_checkequal(f.color_map(3,:), [0, 1, 0]);
delete(f);

// Non - Existing color
f = figure("backgroundcolor", [0.5 0.2 0.6]);
a = gca();
assert_checkequal(f.background, 34);
assert_checkequal(a.background, 33);
assert_checkequal(a.filled, "off");
assert_checkequal(size(f.color_map), [34, 3]);
assert_checkequal(f.color_map(34,:), [0.5 0.2 0.6]);
delete(f);

