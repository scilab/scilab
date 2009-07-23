// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4615 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4615
//
// <-- Short Description -->
// xs2jpg does not consider .jpeg as a valid extension for .jpg

plot3d()
filename  = pathconvert(TMPDIR+"/mybug_4615.jpeg",%F);
my_handle = gcf();
my_win_id = my_handle.figure_id;
xs2jpg(my_win_id,filename);
if fileinfo(filename) == [] then pause, end
mdelete(filename);
