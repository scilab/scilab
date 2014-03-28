// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3009 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3009
//
// <-- Short Description -->
// Default figure and axes handles "user_data" field is not taken into account when creating a new figure.
//

df = gdf();
df.user_data = list(1,2,3);
da = gda();
da.user_data = rand(50,50);

// create a figure and axes and check their user_data fields
fig = gcf();
axes = gca();
if (fig.user_data <> df.user_data) then pause; end
if (axes.user_data <> da.user_data) then pause; end
