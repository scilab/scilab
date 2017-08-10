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
// <-- Non-regression test for bug 15072 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15072
//
// <-- Short Description -->
// the context was wrongly set to the parent diagram
//

scs_m = xcosDiagramToScilab(SCI+"/modules/xcos/tests/nonreg_tests/bug_15072.zcos");

assert_checkequal(scs_m.props.context, ["parent = 1" ; "child1 = 0" ; "child2 = 0"]);
assert_checkequal(scs_m.objs(1).model.rpar.props.context, "child1 = 1");
assert_checkequal(scs_m.objs(2).model.rpar.props.context, "child2 = 1");

xcosDiagramToScilab(TMPDIR+"/sample_after.zcos", scs_m);
clear scs_m
scs_m = xcosDiagramToScilab(TMPDIR+"/sample_after.zcos");

assert_checkequal(scs_m.props.context, ["parent = 1" ; "child1 = 0" ; "child2 = 0"]);
assert_checkequal(scs_m.objs(1).model.rpar.props.context, "child1 = 1");
assert_checkequal(scs_m.objs(2).model.rpar.props.context, "child2 = 1");

