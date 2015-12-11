// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5isAttr", 1);
assert_checkerror("h5isAttr()",msgerr,77);
assert_checkfalse(h5isAttr("42"));
assert_checkfalse(h5isAttr(42));

w = "testing";
x = 1:10;
y = [1 2;3 4];
z = 1;
save(TMPDIR + "/w.sod", "w");
save(TMPDIR + "/x.sod", "x");
save(TMPDIR + "/y.sod", "y");
save(TMPDIR + "/z.sod", "z");

a = h5open(TMPDIR + "/w.sod", "r");
assert_checkequal(h5isAttr(a),%F);
assert_checkequal(h5isAttr(a.root),%F);
assert_checkequal(h5isAttr(a.root.w),%F);
assert_checkequal(h5isAttr(a.root.SCILAB_scilab_version),%T);
h5close(a);

b = h5open(TMPDIR + "/x.sod", "r");
assert_checkequal(h5isAttr(b),%F);
assert_checkequal(h5isAttr(b.root),%F);
assert_checkequal(h5isAttr(b.root.x),%F);
assert_checkequal(h5isAttr(b.root.SCILAB_scilab_version),%T);
h5close(b);

c = h5open(TMPDIR + "/y.sod", "r");
assert_checkequal(h5isAttr(c),%F);
assert_checkequal(h5isAttr(c.root),%F);
assert_checkequal(h5isAttr(c.root.y),%F);
assert_checkequal(h5isAttr(c.root.SCILAB_scilab_version),%T);
h5close(c);

d = h5open(TMPDIR + "/z.sod", "r");
assert_checkequal(h5isAttr(d),%F);
assert_checkequal(h5isAttr(d.root),%F);
assert_checkequal(h5isAttr(d.root.z),%F);
assert_checkequal(h5isAttr(d.root.SCILAB_scilab_version),%T);
h5close(d)
