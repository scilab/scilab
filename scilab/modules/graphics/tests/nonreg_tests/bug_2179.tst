// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2179 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2179
//
// <-- Short Description -->
// Problem with hist3d and %nan. Create blank Screen or crash.

Z = zeros(100,5);
A = abs(rand(40,5));
Z(1:40,:) = A;
scf();
hist3d(Z);
Index = find(Z==0);
Z(Index) = %nan;
scf();
hist3d(Z);
A = abs(rand(10,5));
Z(91:100,:) = A;
scf();
hist3d(Z); 

// all the 3 created figures should display something



