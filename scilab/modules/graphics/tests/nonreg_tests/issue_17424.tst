// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for issue 17424 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17424
//
// <-- Short Description -->
// Labels & title rendering is wrong in forth image in cutaxes documentation

// cutaxes() on an axes not drawn yet: titles must stay automatically placed
f = gcf();
f.immediate_drawing = "off";
x = linspace(-15, 15, 400);
subplot(1,2,1)
plot2d(x, sinc(x).^2)
ax0 = gca();
xtitle("$\LARGE sinc^2$", "Abscissae [a.u.]", "Amplitude")
subplot(1,2,2)
Axes = cutaxes(ax0, "y", [0 0.1 ; 0.1 1], "widths", "-");
f.immediate_drawing = "on";

a = Axes($);
assert_checkequal(a.title.text, "$\LARGE sinc^2$");
assert_checkequal(a.x_label.text, "Abscissae [a.u.]");
assert_checkequal(a.y_label.text, "Amplitude");
assert_checkequal(a.title.auto_position, "on");
assert_checkequal(a.x_label.auto_position, "on");
assert_checkequal(a.y_label.auto_position, "on");

// Rulers of the main axes kept, for the labels to leave room for the tick labels
assert_checkequal(a.axes_visible(1:2), ["on" "on"]);
assert_checkequal(a.font_color, f.background);
assert_checkequal(Axes(1).axes_visible(1:2), ["on" "on"]);

// A position set by the user is still copied
clf
plot2d(x, sinc(x).^2)
ax0 = gca();
ylabel("Amplitude")
ax0.y_label.position = [-14 0.5];
Axes = cutaxes(ax0, "y", [0 0.1 ; 0.1 1], "widths", "-");
a = Axes($);
assert_checkequal(a.y_label.auto_position, "off");
assert_checkequal(a.y_label.position, [-14 0.5]);
