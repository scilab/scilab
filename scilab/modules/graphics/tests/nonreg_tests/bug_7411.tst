// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 7411 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7411
//
// <-- Short Description -->
// clf() forgets to turn off the event handler

f = scf();
plot2d();
f.event_handler = "foo";
f.event_handler_enable = "on";
clf
assert_checkequal(f.event_handler, "foo");
assert_checkequal(f.event_handler_enable, "off");
assert_checkequal(f.info_message, "");

