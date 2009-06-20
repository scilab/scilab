// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2009 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 4592 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4592
//
// <-- Short Description -->
//    The ged entity picker produces an error if a menu has been added to the graphic window

f=scf(0);clf()
xpoly([0 1],[0 1])
addmenu(0,'foo')

mprintf('Activate the ""EDIT/Start entity picker"" then click on the line')
