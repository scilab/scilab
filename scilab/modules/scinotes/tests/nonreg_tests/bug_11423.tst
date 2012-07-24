// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 11423 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11423
//
// <-- Short Description -->
// When select the variable "a" all the a are highlighted rather than only the
// variable "a"

mputl(['function s=foo(a)', 'a=type(a)', 's=''scilab type'''], TMPDIR + '/bug_11423.sce')
scinotes(TMPDIR + '/bug_11423.sce');

// Select the variable "a", the a in scilab must not be highlighted
// Select the function type, the type in the string must not be highlighted.