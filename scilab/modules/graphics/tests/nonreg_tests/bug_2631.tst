// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2631 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2631
//
// <-- Short Description -->
// Change the <figure_handle>.figure_name property not modify the window title like
// with scilab-4.1.2
// 

name = 'Something';
my_figure_handle             = scf(10);
my_figure_handle.figure_name = name;

if (my_figure_handle.figure_name <> name) then pause; end;



