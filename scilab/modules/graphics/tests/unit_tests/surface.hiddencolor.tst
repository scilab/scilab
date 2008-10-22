// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Create a surface
surf();

// get the handle on the surface
srf = gce();

// set hidden color to some value and check them
srf.hiddencolor = 0;
if (srf.hiddencolor <> 0) then pause; end

srf.hiddencolor = -1;
if (srf.hiddencolor <> -1) then pause; end

srf.hiddencolor = -2;
if (srf.hiddencolor <> -2) then pause; end

// get colormap size
fig = gcf();
cmapSize = size(fig.color_map);
cmapSize = cmapSize(1);

srf.hiddencolor = cmapSize;
if (srf.hiddencolor <> cmapSize) then pause; end


