// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Manuel Juliachs
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 7188 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7188
//
// <-- Short Description -->
// Deleting a Label causes Scilab to crash
//

plot3d();
a = gca();

ierr = execstr("delete(a.title)", "errcatch");
if ierr == 0 then pause, end
ierr = execstr("delete(a.x_label)", "errcatch");
if ierr == 0 then pause, end
ierr = execstr("delete(a.y_label)", "errcatch");
if ierr == 0 then pause, end
ierr = execstr("delete(a.z_label)", "errcatch");
if ierr == 0 then pause, end

// Deleting a label used to crash Scilab
// which should not occur now: Label objects cannot be
// directly deleted anymore.
