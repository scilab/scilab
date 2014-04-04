// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test ticks_format & ticks_st (axes properties)

plot([69;70], [66.425514; 66.425513])
a=gca();
assert_checkequal(a.x_ticks.labels(1), string(69));
a.ticks_format="%.2f";
assert_checkequal(a.x_ticks.labels(1), sprintf("%.2f", 69));
a.ticks_format="%.3e";
assert_checkequal(a.x_ticks.labels(1), sprintf("%.3e", 69));
a.ticks_format="hello %.3e scilab";
assert_checkequal(a.x_ticks.labels(1), sprintf("hello %.3e scilab", 69));

a.ticks_st=[1, 0; 1 66.425513]';
a.ticks_format=["" "%.2e"];
assert_checkequal(a.y_ticks.labels(1), sprintf("%.2e", 0));
a.ticks_st=[1, 0; 1e6 66.425513]';
assert_checkequal(a.y_ticks.labels($), sprintf("%.2e", 1.2));
