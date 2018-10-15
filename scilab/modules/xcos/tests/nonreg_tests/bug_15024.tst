// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Clement DAVID
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15024 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15024
//
// <-- Short Description -->
// the labels were not preserved when reloading a block
//

loadXcosLibs();
scicos_log("TRACE");

scs_m = xcosDiagramToScilab(SCI + "/modules/xcos/tests/nonreg_tests/bug_15024.zcos");

// link labels
assert_checkequal(scs_m.objs(4).id, "link1");
assert_checkequal(scs_m.objs(5).id, "link2");

// labels inside the superblock
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.id, "bigsom_label")
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.id, "input1")
assert_checkequal(scs_m.objs(3).model.rpar.objs(3).graphics.id, "input2")

// save a copy and recheck
xcosDiagramToScilab(TMPDIR + "/bug_15024.zcos", scs_m);
scs_m = xcosDiagramToScilab(TMPDIR + "/bug_15024.zcos");

// link labels
assert_checkequal(scs_m.objs(4).id, "link1");
assert_checkequal(scs_m.objs(5).id, "link2");

// labels inside the superblock
assert_checkequal(scs_m.objs(3).model.rpar.objs(1).graphics.id, "bigsom_label")
assert_checkequal(scs_m.objs(3).model.rpar.objs(2).graphics.id, "input1")
assert_checkequal(scs_m.objs(3).model.rpar.objs(3).graphics.id, "input2")

