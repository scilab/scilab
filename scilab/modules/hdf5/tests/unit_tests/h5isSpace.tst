// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5isSpace", 1);
assert_checkerror("h5isSpace()",msgerr,77);
assert_checkfalse(h5isSpace(42));
assert_checkfalse(h5isSpace("42"));

w = "testing";
x = 1:10;
y = [1 2;3 4];
z = 1;
save(TMPDIR + "/w.sod", "w");
save(TMPDIR + "/x.sod", "x");
save(TMPDIR + "/y.sod", "y");
save(TMPDIR + "/z.sod", "z");

a = h5open(TMPDIR + "/w.sod", "r");
assert_checkfalse(h5isSpace(a));
assert_checkfalse(h5isSpace(a.root));
assert_checkfalse(h5isSpace(a.root.w));
assert_checktrue(h5isSpace(a.root.w.dataspace));
h5close(a);

b = h5open(TMPDIR + "/x.sod", "r");
assert_checkfalse(h5isSpace(b));
assert_checkfalse(h5isSpace(b.root));
assert_checkfalse(h5isSpace(b.root.x));
assert_checktrue(h5isSpace(b.root.x.dataspace));
h5close(b);

c = h5open(TMPDIR + "/y.sod", "r");
assert_checkfalse(h5isSpace(c));
assert_checkfalse(h5isSpace(c.root));
assert_checkfalse(h5isSpace(c.root.y));
assert_checktrue(h5isSpace(c.root.y.dataspace));
h5close(c);

d = h5open(TMPDIR + "/z.sod", "r");
assert_checkfalse(h5isSpace(d));
assert_checkfalse(h5isSpace(d.root));
assert_checkfalse(h5isSpace(d.root.z));
assert_checktrue(h5isSpace(d.root.z.dataspace));
h5close(d);
