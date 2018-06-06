// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15598 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15598
//
// <-- Short Description -->
// string(handle) returns "" instead of calling %h_string()  (regression)

deff('out=%h_string(hdl)','out=hdl.type')
assert_checkequal(string(gcf()),'Figure');
assert_checkequal(string(gca()),'Axes');
