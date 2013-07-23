// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5isSet", 1);
assert_checkerror("h5isSet()",msgerr,77);
assert_checkfalse(h5isSet("42"));
assert_checkfalse(h5isSet(42));

w = "testing";
x = 1:10;
y = [1 2;3 4];
z = 1;
save(TMPDIR + "/w.sod", "w");
save(TMPDIR + "/x.sod", "x");
save(TMPDIR + "/y.sod", "y");
save(TMPDIR + "/z.sod", "z");

a = h5open(TMPDIR + "/w.sod", "r");
assert_checkfalse(h5isSet(a));
assert_checktrue(h5isSet(a.root.w));
h5close(a);

b = h5open(TMPDIR + "/x.sod", "r");
assert_checkfalse(h5isSet(b));
assert_checktrue(h5isSet(b.root.x));
h5close(b);

c = h5open(TMPDIR + "/y.sod","r");
assert_checkfalse(h5isSet(c));
assert_checkfalse(h5isSet(c.root));
assert_checktrue(h5isSet(c.root.y));
h5close(c);

d = h5open(TMPDIR + "/z.sod","r");
assert_checkfalse(h5isSet(d));
assert_checktrue(h5isSet(d.root.z));
h5close(d);
