// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

scs_m = scicos_diagram();

Sum1 = BIGSOM_f("define");
Sum2 = BIGSOM_f("define");
lnk1 = scicos_link( from=[1 1 0],to=[2 1 1]);
lnk2 = scicos_link();

// 'Sum1' is linked to 'Sum2' through 'lnk1'
scs_m.objs(1) = Sum1;
scs_m.objs(2) = Sum2;
scs_m.objs(3) = lnk1;
scs_m.objs(4) = lnk2;

// Consecutively delete the diagram objects,
// check that they were replaced with "Deleted" mlists.
scs_m.objs(1)=scicos_new("Deleted");
assert_checkequal(scs_m.objs(1), mlist("Deleted"));

scs_m.objs(2)=scicos_new("Deleted");
assert_checkequal(scs_m.objs(2), mlist("Deleted"));

scs_m.objs(3)=scicos_new("Deleted");
assert_checkequal(scs_m.objs(3), mlist("Deleted"));

scs_m.objs(4)=scicos_new("Deleted");
assert_checkequal(scs_m.objs(4), mlist("Deleted"));

// Test that all objects are simultaneously "deleted"
assert_checkequal(scs_m.objs(1), mlist("Deleted"));
assert_checkequal(scs_m.objs(2), mlist("Deleted"));
assert_checkequal(scs_m.objs(3), mlist("Deleted"));
assert_checkequal(scs_m.objs(4), mlist("Deleted"));


// Check that all the model items are freed
clear
