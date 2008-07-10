// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3085 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3085
//
// <-- Short Description -->
// If figure_size property is modified more than one time, next changes have no effect on the figure.

my_handle = scf();
my_handle.figure_size = [400,400] ;  // OK
my_handle.figure_size = [400,600];   // used to failed
if (my_handle.figure_size <> [400, 600]) then pause; end;

