// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3306 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3306
//
// <-- Short Description -->
// Automatic completion swallows the first completed letter when using double quotes (") around a string, while it gives good 

// <-- INTERACTIVE TEST -->

unix('touch foofile.ext')

scipad('foof [TAB]
// it should be scipad('foofile.ext

scipad("foof [TAB]
// it should be scipad("foofile.ext
                    