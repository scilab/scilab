// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 5616 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5616
//
// <-- Short Description -->
// typeof(uiCreateTree(...)) returns "Tree" instead of "uitree"

l1 = uiCreateNode("CONST_m","default","");
root1 = uiCreateNode("Sources");
tree = uiCreateTree(root1,l1);
assert_checkequal(typeof(tree),"uitree");
assert_checkequal(typeof(l1),"uitree");
