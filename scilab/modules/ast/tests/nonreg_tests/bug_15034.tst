// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 15034 -->
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15034
//
// <-- Short Description -->
// Unable to create a structure using mlist.

a = mlist(["st", "dims", "champs"], int32([1 1]));
disp(a);
