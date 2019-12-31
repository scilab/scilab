// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018, 2019 - Samuel GOUGEON
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
assert_checkequal(iscolor("red", "1"), [%nan %nan %nan]);
assert_checkequal(iscolor("red", "."), [%nan %nan %nan]);
assert_checkequal(iscolor("red", "#"), [%nan %nan %nan]);
assert_checkequal(iscolor("orange"), [255 165 0]/255);
assert_checkequal(iscolor("OrAngE"), [255 165 0]/255);
assert_checkequal(iscolor("Scilab Blue2"), [0 0 208]/255);
cnames = ["r" "g" "GRE" "b" "c" "m" "ma" "y" "Yel" "w" "k"];
ref =     [1.  0.   0.   0.  0.  1.  1.   1.   1.   1.   0.
           0.  1.   1.   0.  1.  0.  0.   1.   1.   1.   0.
           0.  0.   0.   1.  1.  1.  1.   0.   0.   1.   0.
           ]';
assert_checkequal(iscolor(cnames), ref);

// #RRGGBB
assert_checkequal(iscolor("#123456"), [18 52 86]/255);
assert_checkequal(iscolor("#123456"), [18 52 86]/255);
assert_checkequal(iscolor("#abcdef"), [171 205 239]/255);
assert_checkequal(iscolor("#ABcDeF"), [171 205 239]/255);
assert_checkequal(iscolor("#12345G"), [%nan %nan %nan]);
assert_checkequal(iscolor("#1234"),   [%nan %nan %nan]);

// Mixed names + #RRGGBB
assert_checkequal(iscolor(["#abcdef" "orange"]), [171 205 239 ; 255 165 0]/255);

// Indices
sdf();
assert_checkequal(iscolor([-1 -2]), [-1 ; -2]); // special indices
assert_checkequal(iscolor(0), %nan);
assert_checkequal(iscolor([-1 -2 0]), [-1 -2 %nan]');
assert_checkequal(iscolor(1.5), %nan);
assert_checkequal(iscolor(4), 4);
assert_checkequal(iscolor(4, "a"), %nan);
assert_checkequal(iscolor(4, "."), %nan);
assert_checkequal(iscolor(4, "#"), %nan);
assert_checkequal(iscolor(100), %nan);  // too big for the default CM
assert_checkequal(iscolor(1:2), (1:2)');
assert_checkequal(iscolor(1:3), (1:3)');

f = scf(123);
f.color_map = jetcolormap(64);
assert_checkequal(iscolor([1 7 4 3]), [1 7 4 3]');
assert_checkequal(iscolor(65), %nan);
delete(f)

// [r g b]
assert_checkequal(iscolor([-0.1 .2 .3]), [%nan %nan %nan]);
assert_checkequal(iscolor([0.1 .2  3]), [%nan %nan %nan]);
assert_checkequal(iscolor([0.1 .2 .3]), [0.1 0.2 0.3]);
assert_checkequal(iscolor([0.1 .2 .3], "a"), %nan*ones(1,3));
assert_checkequal(iscolor([0.1 .2 .3], "#"), %nan*ones(1,3));
assert_checkequal(iscolor([0.1 .2 .3], "1"), %nan*ones(1,3));
assert_checkequal(iscolor([0.1 .2 .3], "1."), [.1 .2 .3]);
assert_checkequal(iscolor([0.1 .2 .3 .4]), %nan*ones(4,1));
m = rand(10,3);
assert_checkequal(iscolor(m), m);

