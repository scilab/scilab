// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2042 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2042
//
// <-- Short Description -->
// Bug copying entity have different behaviour.

subplot(211);
plot2d();
e = gce();
subplot(212);
a = gca();
// Save axes properties that should not be modified.
axes_visible = a.axes_visible;
box = a.box;
margins = a.margins;
x_label_visible = a.x_label.visible;
y_label_visible = a.y_label.visible;
z_label_visible = a.z_label.visible;
// Now copy handle.
copy(e);
a = gca();

// Check that axes properties did not change.
if( or(a.axes_visible ~= axes_visible) ) then pause; end
if( a.box ~= box ) then pause; end
if( or(a.margins ~= margins) ) then pause; end
if( x_label_visible ~= a.x_label.visible ) then pause; end
if( y_label_visible ~= a.y_label.visible ) then pause; end
if( z_label_visible ~= a.z_label.visible ) then pause; end

delete(gcf());

subplot(211);
plot3d();
e = gce();
subplot(212);
a = gca();
// Save axes properties that should not be modified.
axes_visible = a.axes_visible;
box = a.box;
margins = a.margins;
x_label_visible = a.x_label.visible;
y_label_visible = a.y_label.visible;
z_label_visible = a.z_label.visible;
// Now copy handle.
copy(e);
a = gca();

// Check that axes properties did not change.
if( or(a.axes_visible ~= axes_visible) ) then pause; end
if( a.box ~= box ) then pause; end
if( or(a.margins ~= margins) ) then pause; end
if( x_label_visible ~= a.x_label.visible ) then pause; end
if( y_label_visible ~= a.y_label.visible ) then pause; end
if( z_label_visible ~= a.z_label.visible ) then pause; end