// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15266 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15266
//
// <-- Short Description -->
// Wrong default out_mode in linear_interpn

a=[1 1 1 1];
b=[1 1 1 1];
c=[1 1 1 1];
d=[-10 -9 -8];
e=[0 0.1 0.2];
f=[0 1 2];
g(:,:,1)=[1 2 3;4 5 6;7 8 9];
g(:,:,2)=[1 2 3;4 5 6;7 8 9];
g(:,:,3)=[1 2 3;4 5 6;7 8 9];
 
assert_checkequal(linear_interpn(a,b,c,d,e,f,g), [9 9 9 9]);

