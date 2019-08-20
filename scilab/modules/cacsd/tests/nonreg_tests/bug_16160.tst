// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16160 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16160
//
// <-- Short Description -->
// ppol changes values of third input variable


A=rand(6,6);
B=rand(6,1);
p=[1+%i*0.5 1-%i*0.5 3 -1+%i*0.5 -1-%i*0.5 5];
K=ppol(A,B,p);
assert_checkequal(p,[1+%i*0.5 1-%i*0.5 3 -1+%i*0.5 -1-%i*0.5 5]);