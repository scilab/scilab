// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2134-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2134
//
// <-- Short Description -->
// drawlater causes wrong placement of subplot titles for logarithmic plot2d
// 

scf();
subplot(121);
axes1 = gca();
plot2d("oll",1:10,1:10);xtitle("Plot1","firstX","firstY")

subplot(122);
axes2 = gca();
plot2d("oll",1:10,1:10);xtitle("Plot2","secondX","secondY")

scf();
clf();
drawlater
subplot(121);
axes3 = gca();
plot2d("oll",1:10,1:10);xtitle("Plot1","firstX","firstY")
subplot(122);
axes4 = gca();
plot2d("oll",1:10,1:10);xtitle("Plot2","secondX","secondY")
drawnow

// check that label positions are the same between axes1 and axes3 and between axes2 and axes4
if abs(axes1.title.position - axes3.title.position) > 6e-3  then pause; end
if abs(axes1.title.font_angle - axes3.title.font_angle) > %eps then pause; end

if abs(axes1.x_label.position - axes3.x_label.position) > %eps then pause; end
if abs(axes1.x_label.font_angle - axes3.x_label.font_angle) > %eps then pause; end

if abs(axes1.y_label.position - axes3.y_label.position) > %eps then pause; end
if abs(axes1.y_label.font_angle - axes4.y_label.font_angle) > %eps then pause; end

if abs(axes1.z_label.position - axes3.z_label.position) > %eps then pause; end
if abs(axes1.z_label.font_angle - axes3.z_label.font_angle) > %eps then pause; end

if abs(axes2.title.position - axes4.title.position) > %eps then pause; end
if abs(axes2.title.font_angle - axes4.title.font_angle) > %eps then pause; end

if abs(axes2.x_label.position - axes4.x_label.position) > %eps then pause; end
if abs(axes2.x_label.font_angle - axes4.x_label.font_angle) > %eps then pause; end

if abs(axes2.y_label.position - axes4.y_label.position) > %eps then pause; end
if abs(axes2.y_label.font_angle - axes4.y_label.font_angle) > %eps then pause; end

if abs(axes2.z_label.position - axes4.z_label.position) > %eps then pause; end
if abs(axes2.z_label.font_angle - axes4.z_label.font_angle) > %eps then pause; end

