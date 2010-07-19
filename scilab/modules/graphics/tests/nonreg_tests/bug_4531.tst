// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4531 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4531
//
// <-- Short Description -->
// "legend" macros does show any error when no 'Polyline' exist.


scf(42);
ierr=execstr("legend(""text"")", "errcatch");
if ierr==0 then pause, end;
