// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16498 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16498
//
// <-- Short Description -->
// After esb=sparse([%t %f]); esb(1,:)=[]; , size(esb) answers [0 2] instead of [0 0]


esb=sparse([%t %f]); 
esb(1,:)=[];
assert_checkequal(size(esb),[0,0])

esb=sparse([%t;%f]); 
esb(:,1)=[];
assert_checkequal(size(esb),[0,0])