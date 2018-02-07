// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// Unitary tests of iscolor()

// Names
assert_checkequal(iscolor([]), []);
assert_checkequal(iscolor("red"), [1 0 0]);
assert_checkequal(iscolor("red", "a"), [1 0 0]);
assert_checkequal(iscolor("red", "1"), [-1 -1 -1]);
assert_checkequal(iscolor("red", "."), [-1 -1 -1]);
assert_checkequal(iscolor("red", "#"), [-1 -1 -1]);
assert_checkequal(iscolor("orange"), [255 165 0]/255);
assert_checkequal(iscolor("OrAngE"), [255 165 0]/255);
assert_checkequal(iscolor("Scilab Blue2"), [0 0 208]/255);

// #RRGGBB
assert_checkequal(iscolor("#123456"), [18 52 86]/255);
assert_checkequal(iscolor("#123456"), [18 52 86]/255);
assert_checkequal(iscolor("#12345G"), [-1 -1 -1]);
assert_checkequal(iscolor("#abcdef"), [171 205 239]/255);
assert_checkequal(iscolor("#ABcDeF"), [171 205 239]/255);

// Mixed names + #RRGGBB
assert_checkequal(iscolor(["#abcdef" "orange"]), [171 205 239 ; 255 165 0]/255);

// Indices
cmap = gdf().color_map;
assert_checkequal(iscolor(-1), [-1 -1 -1]);
assert_checkequal(iscolor(0), [-1 -1 -1]);
assert_checkequal(iscolor(1.5), [-1 -1 -1]);
assert_checkequal(iscolor(4), cmap(4,:));
assert_checkequal(iscolor(4, "a"), [-1 -1 -1]);
assert_checkequal(iscolor(4, "."), [-1 -1 -1]);
assert_checkequal(iscolor(4, "#"), [-1 -1 -1]);
assert_checkequal(iscolor(100), [-1 -1 -1]);
assert_checkequal(iscolor(1:2), cmap(1:2,:));
assert_checkequal(iscolor(1:3), [-1 -1 -1]);    // warning
assert_checkequal(iscolor(1:3, "1"), cmap(1:3,:));
assert_checkequal(iscolor(1:4), cmap(1:4,:));

f = scf(123);
cmap = jetcolormap(64);
f.color_map = cmap;
assert_checkequal(iscolor(4), cmap(4,:));
assert_checkequal(iscolor(1:2), cmap(1:2,:));
assert_checkequal(iscolor(1:3, "1"), cmap(1:3,:));
assert_checkequal(iscolor(1:4), cmap(1:4,:));
assert_checkequal(iscolor(65), [-1 -1 -1]);
delete(f)

// [r g b]
assert_checkequal(iscolor([-0.1 .2 .3]), [-1 -1 -1]);
assert_checkequal(iscolor([0.1 .2  3]), [-1 -1 -1]);
assert_checkequal(iscolor([0.1 .2 .3]), [0.1 0.2 0.3]);
assert_checkequal(iscolor([0.1 .2 .3], "a"), -ones(3,3));
assert_checkequal(iscolor([0.1 .2 .3], "#"), -ones(3,3));
assert_checkequal(iscolor([0.1 .2 .3], "1"), -ones(3,3));
assert_checkequal(iscolor([0.1 .2 .3], "1."), [.1 .2 .3]);
assert_checkequal(iscolor([0.1 .2 .3 .4]), -ones(4,3));
m = rand(10,3);
assert_checkequal(iscolor(m), m);

