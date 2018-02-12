// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15413 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15413
//
// <-- Short Description -->
// Deletion of ranges of a structures array failed: s(3,4).r = 1;  s(:,2) = []  

s0(4,5).r = 1;
s = s0; s(:,3) = [];
assert_checkequal(size(s),[4 4]);
s = s0; s(:,[1 3]) = [];
assert_checkequal(size(s),[4 3]);
s = s0; s(:,$) = [];
assert_checkequal(size(s),[4 4]);
s = s0; s(:,$-1) = [];
assert_checkequal(size(s),[4 4]);

s = s0; s(3,:) = [];
assert_checkequal(size(s),[3 5]);
s = s0; s([1 3],:) = [];
assert_checkequal(size(s),[2 5]);
s = s0; s($,:) = [];
assert_checkequal(size(s),[3 5]);
s = s0; s($-1,:) = [];
assert_checkequal(size(s),[3 5]);

// With an hyperarray
s0(4,5,6).r = 1;
s = s0; s(:,3,:) = [];
assert_checkequal(size(s),[4 4 6]);
s = s0; s(:,[1 3],:) = [];
assert_checkequal(size(s),[4 3 6]);
s = s0; s(:,$,:) = [];
assert_checkequal(size(s),[4 4 6]);
s = s0; s(:,$-1,:) = [];
assert_checkequal(size(s),[4 4 6]);

s = s0; s(:,:,1) = [];
assert_checkequal(size(s),[4 5 5]);
s = s0; s(:,:,[2 4]) = [];
assert_checkequal(size(s),[4 5 4]);
s = s0; s(:,:,$) = [];
assert_checkequal(size(s),[4 5 5]);
s = s0; s(:,:,$-1) = [];
assert_checkequal(size(s),[4 5 5]);

