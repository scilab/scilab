// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1201-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3991
//
// <-- Short Description -->
// Legends are not updated when observed polylines are modified.
// 

// create a polylines
plot(1:10);
p1 = gce();
p1 = p1.children(1);

// add a legend for it
leg = legend(["p1"]);

// check the link
if (leg.links(1) <> p1) then pause; end
if (leg.links(1).foreground <> p1.foreground) then pause; end

// modify p1 property
p1.foreground = 5;
// p1.links(1) should be p1
if (leg.links(1) <> p1) then pause; end
if (leg.links(1).foreground <> 5) then pause; end
