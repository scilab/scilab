// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for issue 17554 -->
//
// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHICS -->
//
// <-- Gitlab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17554
//
// <-- Short Description -->
// legend(..).font_size = 3: unexpected error or/and crash

// single scalar string argument must not be read as legend.abc
clf(); plot([1:10;1:10]');
legend("abc").font_size = 3;
assert_checkequal(gce().font_size, 3);

clf(); plot([1:10;1:10]');
legend("abc", "def").font_size = 4;
assert_checkequal(gce().font_size, 4);

// temporary arguments must survive the call (crash)
clf(); plot([1:10;1:10]');
legend(["abc", "def"]).font_size = 5;
assert_checkequal(gce().font_size, 5);

clf(); plot([1:10;1:10]');
legend("ab" + "c").font_size = 6;
assert_checkequal(gce().font_size, 6);

// a("b") => a.b shorthand must be kept
clear s
s("b") = 1;
assert_checkequal(s.b, 1);
s.c("d") = 2;
assert_checkequal(s.c.d, 2);

h = gca();
h("font_size") = 2;
assert_checkequal(gca().font_size, 2);

gcf()("figure_name") = "issue 17554";
assert_checkequal(gcf().figure_name, "issue 17554");

// a local variable shadows the function of the same name
function r = shadow()
    legend = struct();
    legend("abc") = 7;
    r = legend.abc;
endfunction
assert_checkequal(shadow(), 7);
assert_checkequal(type(legend), 13);
