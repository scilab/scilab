// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- TEST WITH GRAPHICS -->
//
// <-- Non-regression test for bug 14904 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14902
//
// <-- Short Description -->
// "array[k] = []" does not work for handles (Regression)

h=[gca();gcf()];
h(2)=[];
assert_checkequal(size(h),[1,1]);