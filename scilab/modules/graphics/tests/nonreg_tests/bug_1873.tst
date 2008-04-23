// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1873 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1873
//
// <-- Short Description -->
// grid space between dash dash line elements is filled with white 
// background color.


xv = 1:10; yv=xv; s = rand(length(xv), length(yv));
min_z = min(s); max_z = max(s);
plot3d1(xv,yv,s, flag=[-1 1 4], ebox = [min(xv), max(xv), min(yv), max(yv), min_z, max_z]);
a=gca(); a.tight_limits = "on"; a.background = 3; //make nice view
a.grid = [5 5];  
             




