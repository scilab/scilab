// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2953 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2953
//
// <-- Short Description -->
// we lost axes with plot3d and box = 2

t=[0:0.1:2*%pi]'; z=sin(t)*cos(t'); 
f=gcf();f.color_map=hsvcolormap(64);
plot3d1(t,t,z,35,45,"X@Y@Z",[-2,2,2])

// with box = 2 we should have only the back thriedron drawn 
// and no labels
axes = gca();
if (axes.box <> "hidden_axes") then pause; end
if (axes.x_label.visible <> "off") then pause; end
if (axes.y_label.visible <> "off") then pause; end
if (axes.z_label.visible <> "off") then pause; end

// check also that the labels are set even if not drawn
if (axes.x_label.text <> "X") then pause; end
if (axes.y_label.text <> "Y") then pause; end
if (axes.z_label.text <> "Z") then pause; end


