// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15778 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15778
//
// <-- Short Description -->
// Deleting some rows or columns of a boolean sparse array with =[]  crashes Scilab (Regression)

bs = sprand(4,4,0.5)>0.3
bs(:,1) = []                   

bs = sprand(4,4,0.5)>0.3
bs(:,[1 3]) = []               

bs = sprand(4,4,0.5)>0.3
bs(:,$) = []                   

bs = sprand(4,4,0.5)>0.3
bs(1,:) = []                   

bs = sprand(4,4,0.5)>0.3
bs([1 3],:) = []               

bs = sprand(4,4,0.5)>0.3
bs($,:) = []                   