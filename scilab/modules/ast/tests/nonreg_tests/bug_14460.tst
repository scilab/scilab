// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14460 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14460
//
// <-- Short Description -->
// sparse boolean indices were not supported any more

s=speye(2,2);
assert_checkequal(s(s==1),sparse([1;1]) );
I=eye(2,2);
assert_checkequal(I(s==1),[1;1]);
k=(s==0);
s(k)=1;
assert_checkequal(s,sparse([1 1;1 1]));
I(k)=1;
assert_checkequal(I,[1 1;1 1]);


