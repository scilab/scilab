// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- Non-regression test for bug 8726 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8726
//
// <-- Short Description -->
// The karmarkar function may produce a division-by-zero error.
//



Aeq = [
1 -1 0
1  1 1
];
beq = [0;2];
c = [-1;-1;0];
x0 = [1;1;0];
xopt=karmarkar(Aeq,beq,c,x0);
xexpected = [1;1;0];
assert_checkequal ( xopt , xexpected );


