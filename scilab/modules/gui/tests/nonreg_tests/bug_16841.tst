// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHICS -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 16841 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16841
//
// <-- Short Description -->
// In multiaxes, when the user zooms in a non common data area

x = 0:0.1:4*%pi;
y1 = cos(x);
y2 = 3*sin(x);

f = scf();
a1 = gca();
a2 = newaxes();

sca(a1);
plot(x, y1, "b");
sca(a2);
plot(x, y2, "r");

a1.tight_limits = "on";

a2.tight_limits = "on";
a2.box = "off";
a2.y_location = "right";
a2.x_location = "top";
a2.x_label.visible = "off";
a2.filled = "off";

//try to zoom with mousewheel in Y area [-3,-1[ or ]1,3], zoom is applied only on one axes.
