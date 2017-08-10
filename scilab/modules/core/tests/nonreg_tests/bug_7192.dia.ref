// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7192 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7192
//
// <-- Short Description -->
// From S=[], S($+1,:) = some_row inserted it in row#2 after a parasitic row#1

S = [];
S($+1,:) = [ 1 2 ];
assert_checkequal(size(S),[1 2]);
assert_checkequal(S(1,:),[1 2]);

S = [];
S($+1,:) = [ "a" "b" ];
assert_checkequal(size(S),[1 2]);
assert_checkequal(S(1,:),["a" "b"]);

S = [];
S($+1,:) = [ %t %f ];
assert_checkequal(size(S),[1 2]);
assert_checkequal(S(1,:),[%t %f]);

S = [];
S($+1,:) = [ %z %z];
assert_checkequal(size(S),[1 2]);
assert_checkequal(S(1,:),[%z %z]);
