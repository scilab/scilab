// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// non regression bug for plot2d
// which were erasing figure before each plot

// two plot2d
plot2d;
plot2d;

a = gca();
// check that there are now two compounds ad axes children
if ( size(a.children,'*') <> 2 ) then pause,end
if ( a.children(1).type <> "Compound"  ) then pause,end
if ( a.children(2).type <> "Compound"  ) then pause,end
