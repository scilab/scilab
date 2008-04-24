// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2568 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2568
//
// <-- Short Description -->
// The arrows are not drawn in 3D when one have arrowed segs or arrowed polylines
// merged with a surface.
// 


plot3d()
xsegs([0;0],[0;0]);
e=gce();
e.data(:,3)=[0;4];
e.arrow_size=4   

// check that the arrow head is drawn

