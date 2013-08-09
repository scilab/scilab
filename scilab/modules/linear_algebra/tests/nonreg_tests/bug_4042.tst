// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4042 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4042
//
// <-- Short Description -->
//    squeeze(A(1,1,:)) returned an hypermatrix instead a matrix
// =============================================================================

A=ones(3,3);
A(:,:,2)=3*ones(3,3);
A(:,:,3)=-1*ones(3,3);
B=squeeze(A(1,1,:));

assert_checkequal(type(B), 1);

// In Scilab 6, the hypermatrix type won't exist
v = getversion('scilab');
if v(1) > 5 then
    C(:,:,1) = zeros(32,32);
    C(:,:,2) = zeros(32,32);
    assert_checkequal(typeof(C(:,:,1)), "constant");
    assert_checkequal(typeof(C(1,:,:)), "constant");
end
