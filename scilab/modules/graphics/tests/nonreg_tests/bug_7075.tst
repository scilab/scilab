// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- TEST WITH GRAPHIC -->
// <-- Non-regression test for bug 7075 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7075
//
// <-- Short Description -->
// When we set "links" property for a handle of type legend, the property type was not checked.
//


// Test handle type.
plot2d();
legend("A", "B", "C");
e=gce();
ierr = execstr("set(e, ''links'', [1,2,3]);", "errcatch");
if lasterror() <> msprintf(gettext("Wrong type for ''%s'' property: Graphic handle array expected.\n"), "links") then pause,end

close(gcf());

// Test hierarchy.
scf(0);
plot2d();
legend("a", "b", "c");
e0 = gce(); // e0 is the handle of the legend from figure 0.

scf(1);
plot2d();
legend("A", "B", "C");
e1 = gce(); // e1 is the handle of the legend from figure 0.

ierr = execstr("set(e0,''links'',e1.links);", "errcatch");
if lasterror() <> msprintf(gettext("%s: Input argument and the legend must have the same parent axes.\n"), "links") then pause, end
