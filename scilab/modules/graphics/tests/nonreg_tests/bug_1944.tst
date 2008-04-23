// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1944 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1944
//
// <-- Short Description -->
// Y ticks labels are displaced when the graphic window is copied
// in the clipboard as bitmap

plot(1:10);
plot(0:9);
fig = gcf();
// export to bitmap and check the result
clipboard(fig.figure_id, "EMF");

