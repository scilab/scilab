// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - bruno.jofret@scilab-enterprises.com
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 11485 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11485
//
// <-- Short Description -->
// delete axes kill reference before try to use it

plot();
a=gca();delete(a);
a=gca();delete(a);
a=gca();delete(a);
//if finish it is OK