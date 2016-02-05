// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1257 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1257
//
// <-- Short Description -->
// When rotation_style is set to multiple, all axes does not
// rotate when modifying rotation_angles property.

// create two plots
f = gcf();
f.rotation_style = "multiple";
subplot(1,2,1)
plot3d()
subplot(1,2,2)
plot3d()

// rotate one axes
axes1 = f.children(1);
axes2 = f.children(2);
axes1.rotation_angles = [20,30];

if (axes2.rotation_angles <> axes1.rotation_angles) then pause; end



