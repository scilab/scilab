// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15346 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15346
//
// <-- Short Description -->
// Rationals Arrays could not be addressed with linearized indices

r = (1-%z)./%z^[1 2 ; 0 3];
r = cat(3,r,2*r);
r11 = r(1,1)
r12 = r(1,2)
r21 = r(2,1)
r22 = r(2,2)
r211 = r(2,1,2)

// Extractions
// -----------
assert_checkequal(r(1,0), r(1,1));
assert_checkequal(r(2,0), r(2,1));
assert_checkequal(r(3,0), r(1,2));
assert_checkequal(r(4,0), r(2,2));
assert_checkequal(r(5,0), r(1,1,2));
assert_checkequal(r(6,0), r(2,1,2));
assert_checkequal(r(7,0), r(1,2,2));
assert_checkequal(r(8,0), r(2,2,2));
assert_checkequal(r($,0), r(2,2,2));
assert_checkequal(r(:,0), r.num(:)./r.den(:));
assert_checkequal(r(3:6,0), r.num(3:6)./r.den(3:6));
sel = 5:$-1;
assert_checkequal(r(sel,0), r.num(sel)./r.den(sel));

// Insertions
// -----------
r0 = r;
objects = list(1+%z-2*%z^2, (1-%z)/(1+%z), 4);
for o = objects
    r = r0;
    r(6,0) = o;
    ref = r0;
    ref(2,1,2) = o;
    assert_checkequal(r, ref);
    //
    r = r0;
    r([4 6],0) = o;
    ref = r0;
    ref(2,2,1) = o;
    ref(2,1,2) = o;
    assert_checkequal(r, ref);
    //
    r = r0;
    r([4 6],0) = [o -o];
    ref = r0;
    ref(2,2,1) = o;
    ref(2,1,2) = -o;
    assert_checkequal(r, ref);
    //
    r = r0;
    r([4 6],0) = [o ; -o];
    ref = r0;
    ref(2,2,1) = o;
    ref(2,1,2) = -o;
    assert_checkequal(r, ref);
    //
    r = r0;
    r($,0) = o;
    ref = r0;
    ref(2,2,2) = o;
    assert_checkequal(r, ref);
    //
    r = r0;
    r(6:$-1,0) = [o ; -o];
    ref = r0;
    ref(2,1,2) = o;
    ref(1,2,2) = -o;
    assert_checkequal(r, ref);
    //
end
