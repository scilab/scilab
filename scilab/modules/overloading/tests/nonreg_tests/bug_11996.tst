// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 11996 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11996
//
// <-- Short Description -->
// Extend eye() to hypermatrix

ref(:,:,1,1) = eye(6,6);
ref(:,:,2,2) = eye(6,6);
ref(:,:,2,1) = eye(6,6);
ref(:,:,1,2) = eye(6,6);
assert_checkequal(eye(6,6,2,2), ref);
