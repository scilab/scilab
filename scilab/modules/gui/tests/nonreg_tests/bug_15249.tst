// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15249 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/15249
//
// <-- Short Description -->
// findobj("toto") yields an error after https://gitlab.com/scilab/scilab/-/issues/7117

// A property name without value matches nothing: findobj returns []

clf
assert_checkequal(findobj("toto"), []);
assert_checkequal(findobj(gcf(), "toto"), []);
