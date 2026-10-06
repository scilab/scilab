// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// Datatip created at a given position between two vertices must lie on the
// displayed segment, whatever the scale (linear or logarithmic) of each axis.

clf();
x = [1 10 100 1000];
plot(x, x.^2);
a = gca();
e = a.children.children;

// Middle of the displayed segment (10,100)-(100,10000)
flags = ["nn" "ln" "nl" "ll"];
mid   = [55        5050
         10^1.5    5050
         55        10^3
         10^1.5    10^3];

for k = 1:size(flags, "*")
    a.log_flags = flags(k);
    d = datatipCreate(e, mid(k, :));
    assert_checkalmostequal(d.data(1:2), mid(k, :));
    datatipRemove(e, 1);
end

// Datatip created in linear scale, axes switched to log afterwards:
// the datatip stays on the displayed segment
a.log_flags = "nn";
d = datatipCreate(e, [55 5050]);
a.log_flags = "ll";
assert_checkalmostequal(d.data(1:2), [10^1.5 10^3]);
datatipRemove(e, 1);
