// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SCILAB ENTERPRISES - Simon GARESTE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//

msgerr = msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "h5isGroup", 1);
assert_checkerror("h5isGroup()",msgerr,77);
assert_checkfalse(h5isGroup("42"));
assert_checkfalse(h5isGroup(42));

w = "testing";
x = list(1, "hello");

save(TMPDIR + "/w.sod", "w");
save(TMPDIR + "/x.sod", "x");

a = h5open(TMPDIR + "/w.sod", "r");
assert_checkequal(h5isGroup(a),%F);
assert_checkequal(h5isGroup(a.root),%T);
assert_checkequal(h5isGroup(a.root.w),%F);
assert_checkequal(h5isGroup(a.root.w.type),%F);
h5close(a);

b = h5open(TMPDIR + "/x.sod", "r");
assert_checkequal(h5isGroup(b),%F);
assert_checkequal(h5isGroup(b.root),%T);
assert_checkequal(h5isGroup(b.root.x),%T);
h5close(b);