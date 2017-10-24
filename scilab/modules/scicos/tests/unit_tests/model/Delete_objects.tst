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
lnk1 = scicos_link( from=[1 1 0],to=[2 1 1] );
lnk2 = scicos_link();

// 'Sum1' is linked to 'Sum2' through 'lnk1'
scs_m.objs(1) = Sum1;
scs_m.objs(2) = Sum2;
scs_m.objs(3) = lnk1;
scs_m.objs(4) = lnk2;

// Consecutively delete the diagram objects,
// check that they were replaced with "Deleted" mlists
DeletedObj = mlist("Deleted");

scs_m.objs(1) = DeletedObj;
assert_checkequal(scs_m.objs(1), DeletedObj);

scs_m.objs(2) = DeletedObj;
assert_checkequal(scs_m.objs(2), DeletedObj);

scs_m.objs(3) = DeletedObj;
assert_checkequal(scs_m.objs(3), DeletedObj);

scs_m.objs(4) = DeletedObj;
assert_checkequal(scs_m.objs(4), DeletedObj);

// Test that all objects are simultaneously "deleted"
assert_checkequal(scs_m.objs(1), DeletedObj);
assert_checkequal(scs_m.objs(2), DeletedObj);
assert_checkequal(scs_m.objs(3), DeletedObj);
assert_checkequal(scs_m.objs(4), DeletedObj);

// Effectively empty the diagram
for i=3:-1:0
    scs_m.objs($) = null();
    assert_checkequal(size(scs_m.objs), i);
end

// Check that all the model items are freed
clear


// Now effectively delete an object in the midst of other objects
loadXcosLibs();
scs_m = scicos_diagram();

Sum1 = BIGSOM_f("define");
Sum2 = BIGSOM_f("define");
Sum3 = BIGSOM_f("define");
lnk1 = scicos_link( from=[1 1 0],to=[1 1 1] );
lnk2 = scicos_link( from=[4 1 0],to=[4 1 1] );

scs_m.objs(1) = Sum1;
scs_m.objs(2) = lnk1;
scs_m.objs(3) = Sum2; // The one to delete
scs_m.objs(4) = Sum3;
scs_m.objs(5) = lnk2;

// Start by replacing the object with a "Deleted" mlist
DeletedObj = mlist("Deleted");
scs_m.objs(3) = DeletedObj;
assert_checkequal(scs_m.objs(3), DeletedObj);

// Effectively remove the object
scs_m.objs(3) = null();
assert_checkequal(size(scs_m.objs), 4);

// The remaining diagram is not expected to stay coherent, the user should fix it by hand
scs_m.objs(4).from = [3 1 0];
scs_m.objs(4).to = [3 1 1];
assert_checkequal(scs_m.objs(3).graphics.pin, [4; 0]);
assert_checkequal(scs_m.objs(3).graphics.pout, 4);

// Check that all the model items are freed
clear
