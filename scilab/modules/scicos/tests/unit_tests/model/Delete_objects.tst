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
DeletedBlock = mlist("Deleted");

scs_m.objs(1) = DeletedBlock;
assert_checkequal(scs_m.objs(1), DeletedBlock);

scs_m.objs(2) = DeletedBlock;
assert_checkequal(scs_m.objs(2), DeletedBlock);

scs_m.objs(3) = DeletedBlock;
assert_checkequal(scs_m.objs(3), DeletedBlock);

scs_m.objs(4) = DeletedBlock;
assert_checkequal(scs_m.objs(4), DeletedBlock);

// Test that all objects are simultaneously "deleted"
assert_checkequal(scs_m.objs(1), DeletedBlock);
assert_checkequal(scs_m.objs(2), DeletedBlock);
assert_checkequal(scs_m.objs(3), DeletedBlock);
assert_checkequal(scs_m.objs(4), DeletedBlock);


// Check that all the model items are freed
clear
