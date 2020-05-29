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
// <-- Non-regression test for bug 16449 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16449
//
// <-- Short Description -->
// insertion of implicit vector in cell crashes Scilab

b = cell(1,1);
b{1} = 1:3;
disp(b)
