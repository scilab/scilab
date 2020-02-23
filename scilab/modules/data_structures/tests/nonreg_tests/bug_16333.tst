// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16333 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16333
//
// <-- Short Description -->
// tree_show() was broken for Xcos blocks and diagrams


loadXcosLibs();

blk = BIGSOM_f("define");
assert_checkequal(execstr("tree_show(blk)","errcatch"), 0);

//filename = SCI+"/modules/xcos/demos/demo_Datatype.xmi";
//scs_m = scicosDiagramToScilab(filename);
//tree_show(scs_m)
//assert_checkequal(execstr("tree_show(scs_m)","errcatch"), 0);
