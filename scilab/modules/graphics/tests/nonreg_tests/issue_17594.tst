// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for issue 17594 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17594
//
// <-- Short Description -->
// Setting log_flags after Matplot crashes Scilab

msg = msprintf(_("Wrong value for ''%s'' property: Logarithmic scale cannot be used with Matplot.\n"), "log_flags");

// log_flags set after Matplot
clf
Matplot([0 1;2 3])
a = gca();
assert_checkerror("set(a, ""log_flags"", ""ln"")", msg);
assert_checkerror("set(a, ""log_flags"", ""nl"")", msg);
assert_checkequal(a.log_flags, "nnn");

// Matplot inside a compound
clf
Matplot1([1 2;3 4], [1 1 10 10]);
plot(1:10);
glue(gca().children);
assert_checkerror("set(gca(), ""log_flags"", ""ll"")", msg);

// Matplot on axes already in log scale
clf
a = gca();
a.data_bounds = [1 1;10 10];
a.log_flags = "ln";
msg = msprintf(_("%s: Logarithmic scale cannot be used.\n"), "Matplot");
assert_checkerror("Matplot([1 2;3 4])", msg);
assert_checkerror("Matplot(uint8([1 2;3 4]))", msg);
msg = msprintf(_("%s: Logarithmic scale cannot be used.\n"), "Matplot1");
assert_checkerror("Matplot1([1 2;3 4], [1 1 10 10])", msg);
assert_checkequal(a.children, []);
