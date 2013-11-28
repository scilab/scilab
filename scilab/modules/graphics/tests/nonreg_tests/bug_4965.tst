// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4965 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/4965
//
// <-- Short Description -->
// Set links property for a handle of type legend did not work

plot2d();
legend("test1","test2");
e=gce();
e.links;
e.links=e.links;