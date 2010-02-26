// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Allan.Simon@scilab.org
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH EDITOR -->
//
// <-- Non-regression test for bug 4880 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4880
//
// <-- Short Description -->
//    regular expressions with "^" and "$" match only one expression

//  editor()
//  add several line with "aa bb"
//  "Menu→Search→Find/Replace"
//  Find : "^aa"
//  => should hilight all the aa


editor SCI/modules/atoms/macros/atomsSearch.sci
// "Menu→Search→Find/Replace"
// Find    : "atom(.)*earch"
// Replace : "------"
// Check the "Regular expressions" box
// Click on Replace All

