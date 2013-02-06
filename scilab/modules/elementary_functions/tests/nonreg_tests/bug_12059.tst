// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte Hecquet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 12059 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12059
//
// <-- Short Description -->
// Index returned by gsort of sparse are wrong.
//
indref=[2 6 5 3 4 1 8];
[res ind]=gsort(sparse([2 9 6 4 7 8 0 2]));
assert_checkequal(ind,indref);
